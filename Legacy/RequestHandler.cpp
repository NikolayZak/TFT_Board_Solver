#include "RequestHandler.hpp"

RequestHandler::RequestHandler() : database(LOCAL_DB_PATH), TaskSchedular(JOB_EXPIRY_DURATION, JOB_CLEANUP_TIMER){

}

crow::response RequestHandler::handle_compute(const crow::request& req) {
    json body;
    try {
        body = json::parse(req.body);
    } catch (...) {
        return crow::response(400, "Invalid JSON");
    }

    // extract data
    int set_number = body.value("set_number", 0);
    int player_level = body.value("player_level", 0);
    int target_size = body.value("target_size", 0);

    vector<string> champions_added = body.value("champions_added", vector<string>{});
    vector<string> traits_added = body.value("traits_added", vector<string>{});

    // Prepare job
    auto job = [=]() -> vector<BoardResult> {
        SetData set_data = database.allocSet(set_number);
        Solver solver(set_data, 10); // or configurable heap
        solver.UpdateData(set_data, player_level, champions_added, traits_added);
        return solver.Solve(target_size);
    };

    int job_id = TaskSchedular.submit(job);

    json res_json;
    res_json["job_id"] = job_id;

    crow::response res(res_json.dump());
    res.add_header("Content-Type", "application/json");
    return res;
}

crow::response RequestHandler::handle_status(const crow::request& req) {
    // Parse job_id from query string
    auto url_params = crow::query_string(req.url);
    if (!url_params.get("job_id")) {
        return crow::response(400, "Missing job_id parameter");
    }

    int job_id = std::stoi(url_params.get("job_id"));

    // Get job status
    JobStatus status = TaskSchedular.get_status(job_id);

    json res_json;
    res_json["job_id"] = job_id;

    // job still running
    if (status == JobStatus::Running) {
        res_json["status"] = "running";

        crow::response res(res_json.dump());
        res.add_header("Content-Type", "application/json");
        res.add_header("Cache-Control", "no-store");
        return res;
    }

    // Job is finished: try to get result
    optional<vector<BoardResult>> results = TaskSchedular.get_result(job_id);

    // result no longer exists
    if (!results.has_value()) {
        return crow::response(500, "Job finished but result is missing");
    }

    res_json["status"] = "done";

    // Convert vector<BoardResult> to JSON
    json boards_json = json::array();
    for (const auto& board_result : *results) {
        json board_json = json::array();
        for (const auto& name : board_result.board) {
            board_json.push_back(name);
        }
        boards_json.push_back(board_json);
    }

    res_json["data"] = boards_json;

    crow::response res(res_json.dump());
    res.add_header("Content-Type", "application/json");
    res.add_header("Cache-Control", "no-store");
    return res;
}

crow::response RequestHandler::handle_static(const crow::request& req) {
    auto url_params = crow::query_string(req.url_params);
    if (!url_params.get("set_number")) {
        return crow::response(400, "Missing set_number parameter");
    }

    string set_number = url_params.get("set_number");
    string file_path = string(STATIC_DATA_FILE) + "/set" + set_number + ".json";

    ifstream file(file_path);
    if (!file.is_open()) {
        return crow::response(500, "Failed to open " + file_path);
    }
    
    json j;
    try {
        file >> j;
    } catch (const std::exception& e) {
        return crow::response(500, string("JSON parse error: ") + e.what());
    }

    crow::response res;
    res.code = 200;
    res.set_header("Content-Type", "application/json");
    res.body = j.dump();
    return res;
}