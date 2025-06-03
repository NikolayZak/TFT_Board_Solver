#define CROW_USE_BOOST_ASIO
#include "Crow/include/crow.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;
// compile with: g++ test.cpp -I./Crow/include -lboost_system -lpthread -lssl -lcrypto -o test_server



int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/process").methods("POST"_method)([](const crow::request& req) {
        try {
            // Parse JSON body
            auto req_json = json::parse(req.body);

            // Extract values with safety checks
            int set_number = req_json.at("set_number").get<int>();
            int board_size = req_json.at("board_size").get<int>();
            std::vector<std::string> emblems = req_json.at("emblems").get<std::vector<std::string>>();
            std::vector<std::string> champions = req_json.at("champions").get<std::vector<std::string>>();
            int cost_restriction = req_json.at("cost_restriction").get<int>();

            // TODO: Process your data here, e.g.:
            // auto result = do_your_computation(set_number, board_size, emblems, champions, cost_restriction);

            json res_json;
            res_json["status"] = "success";
            res_json["message"] = "Received data processed"; 
            // res_json["result"] = result; // add your real result here

            return crow::response(200, res_json.dump());

        } catch (const std::exception& e) {
            json error_json;
            error_json["status"] = "error";
            error_json["message"] = e.what();
            return crow::response(400, error_json.dump());
        }
    });

    app.port(18080).multithreaded().run();
}

