#include "RequestHandler.hpp"

RequestHandler::RequestHandler(string database_file, string static_folder, int result_expiry_duration, int job_cleanup_timer) : database(database_file), TaskSchedular(result_expiry_duration, job_cleanup_timer){
    saved_static_folder = static_folder;
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

    // Validate inputs
    if (player_level < 1 || player_level > MAX_PLAYER_LEVEL) {
        return crow::response(400, "Invalid player_level: " + to_string(player_level));
    }

    if(target_size < 1 || target_size > MAX_BOARD_SIZE) {
        return crow::response(400, "Invalid target_size: " + to_string(target_size));
    }

    vector<int> valid_sets = database.getSets();
    if(find(valid_sets.begin(), valid_sets.end(), set_number) == valid_sets.end()) {
        return crow::response(400, "Invalid set_number: " + to_string(set_number));
    }

    for(int i = 0; i < champions_added.size(); i++){
        if(!database.isChampionInSet(set_number, champions_added[i])){
            return crow::response(400, "Invalid champions_added: " + champions_added[i]);
        }
    }

    for(int i = 0; i < traits_added.size(); i++){
        if(!database.isTraitInSet(set_number, traits_added[i])){
            return crow::response(400, "Invalid traits_added: " + traits_added[i]);
        }
    }

    // Prepare job
    auto job = [=]() -> vector<BoardResult> {
        try
        {
            SetData set_data = database.allocSet(set_number);
            Solver solver(set_data, 10);
            solver.UpdateData(set_data, player_level, traits_added, champions_added);
            return solver.Solve(target_size);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            vector<BoardResult> empty;
            return empty;
        }
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
    auto url_params = req.url_params;
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
        json board_json;
        board_json["board"] = board_result.board;
        board_json["score"] = board_result.score;

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

    if (!std::all_of(set_number.begin(), set_number.end(), ::isdigit)) {
        return crow::response(400, "Invalid set_number format");
    }

    ifstream file(saved_static_folder + "/" + "set" + set_number + ".json");
    if (!file.is_open()) {
        return crow::response(500, "Failed to open: " + saved_static_folder + "/" + "set" + set_number);
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