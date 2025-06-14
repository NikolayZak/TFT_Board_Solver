/*
    Author: Nikolay Zakirov
    Email: nikolayzakirov31@gmail.com
    Date Created: 2025-06-13
    Last Modified: 2025-06-13

    Description: RequestHandler.hpp
    Responsible to parsing HTML requests and returning HTML responses
*/
#include "LocalDB.hpp"
#include "Solver.hpp"
#include "JobManager.hpp"
#include "../Crow/include/crow.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class RequestHandler{
    private:
    LocalDB database;
    JobManager TaskSchedular;

    public:
    RequestHandler();
    crow::response handle_compute(const crow::request& req); // uses POST method
    crow::response handle_status(const crow::request& req); // uses GET method with no caching
    crow::response handle_static(const crow::request& req); // uses GET method
};