/*
    Author: Nikolay Zakirov
    Email: nikolayzakirov31@gmail.com
    Date Created: 2025-06-13
    Last Modified: 2025-06-13

    Description: RequestHandler.hpp
    Responsible to parsing HTML requests and returning HTML responses
*/
#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "../Crow/include/crow.h"
#include "Common.hpp"
#include "LocalDB.hpp"
#include "Solver.hpp"
#include "JobManager.hpp"

using json = nlohmann::json;
using std::string;
using std::vector;

class RequestHandler{
    private:
    LocalDB database;
    JobManager TaskSchedular;
    string saved_static_folder;

    public:
    RequestHandler(string database_file, string static_folder, int result_expiry_duration, int job_cleanup_timer);
    crow::response handle_compute(const crow::request& req); // uses POST method
    crow::response handle_status(const crow::request& req); // uses GET method with no caching
    crow::response handle_static(const crow::request& req); // uses GET method
};