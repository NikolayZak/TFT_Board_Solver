#include "RequestHandler.hpp"

// compile with g++ FastVector.cpp Common.cpp TopScoringBoards.cpp Serialisation.cpp LocalDB.cpp Board.cpp Solver.cpp JobManager.cpp RequestHandler.cpp api.cpp -lsqlite3 -o api -I../Crow/include

int main() {
    crow::SimpleApp app;
    RequestHandler handler("database.db", "static", 67, 29);

    CROW_ROUTE(app, "/api/compute").methods("POST"_method)
    ([&handler](const crow::request& req){
        return handler.handle_compute(req);
    });

    CROW_ROUTE(app, "/api/job-status").methods("GET"_method)
    ([&handler](const crow::request& req){
        return handler.handle_status(req);
    });

    CROW_ROUTE(app, "/api/static").methods("GET"_method)
    ([&handler](const crow::request& req){
        return handler.handle_static(req);
    });

    app.port(18080).multithreaded().run();
}
