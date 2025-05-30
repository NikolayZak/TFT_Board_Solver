#include "Crow/include/crow.h"

int main()
{
    crow::SimpleApp app;  // Define the Crow app

    // Define a route (GET /)
    CROW_ROUTE(app, "/")([](){
        return "Hello, world!";
    });

    // Define another route (GET /hello/<string>)
    CROW_ROUTE(app, "/hello/<string>")
    ([](std::string name){
        return "Hello, " + name + "!";
    });

    // Start the server on port 18080
    app.port(18080).multithreaded().run();
}
