#include "crow_all.h"

int main()
{
    crow::SimpleApp app;

    //--------------LOGIN ROUTE--------------------------

    CROW_ROUTE(app, "/login").methods("POST"_method)
    ([](const crow::request& req){
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string userName, password, role, token;
        userName = requestBody["username"].s();
        password = requestBody["password"].s();
        // TODO: Do attendance
        // TODO: Return JWT
        response["token"] = "123abc";
        return response;
    });

    //------------------EMPLOYEE ACTIVITY ROUTES-------------------

    //Product Details

    CROW_ROUTE(app, "/product/<int>").methods("GET"_method)
     ([](const crow::request& req, int productID) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string token = requestBody["token"].s();
        // TODO: Return Product Details
        // {name, minPrice, maxPrice, price, stock}
        response["token"] = token;
        response["productID"] = token;
        return response;
     }); 

    // Order Details

        // GET order details
    CROW_ROUTE(app, "/orders/<int>").methods("GET"_method)
     ([](const crow::request& req, int orderID) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string token = requestBody["token"].s();
        // {product, customerName, customerContact, orderStatus}
        crow::json::wvalue x({ {"orderID", orderID} });
        return x;
     });
        //ADD Orders (post)
    CROW_ROUTE(app, "/orders/<int>").methods("POST"_method)
        ([](const crow::request& req) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string token = requestBody["token"].s();
        // {orderID, message}
        crow::json::wvalue x({ {"orderID", "orderID"} });
        return x;
     });
        //REMOVE Orders (delete)
    CROW_ROUTE(app, "/orders/<int>").methods("DELETE"_method)
        ([](const crow::request& req, int orderID) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string token = requestBody["token"].s();
        // {message}
        crow::json::wvalue x({ {"message", orderID} });
        return x;
     });

    //------------------ADMINACTIVITY ROUTES-------------------

    // Inventory Details
        // GET Inventory
    CROW_ROUTE(app, "/inventory/<int>").methods("GET"_method)
        ([] (const crow::request& req, int inventoryID) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string token = requestBody["token"].s();
        // inventoryID is synonymous with productID
        // return whole inventory if inventoryID == 0

        });
        // Update Inventory
        // Using POST instead of PATCH because fuck it
    CROW_ROUTE(app, "/inventory/<int>").methods("POST"_method)
        ([] (const crow::request& req, int inventoryID) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string token = requestBody["token"].s();
        // update inventory    
        });

        // DETELE Inventory
    CROW_ROUTE(app, "/inventory/<int>").methods("DELETE"_method)
        ([](const crow::request& req, int inventoryID) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string token = requestBody["token"].s();
        // delete inventory    
        });

    //Discount Code
        // GET Discount (+ removal route below)
    CROW_ROUTE(app, "/discount").methods("GET"_method)
        ([](const crow::request& req) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string token = requestBody["token"].s();
        // {code, validity, percentage}
     });

    // Sales Data
        // GET Sales Data

    CROW_ROUTE(app, "/sales/<int>").methods("GET"_method)
        ([](const crow::request& req, int timeFrame) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string token = requestBody["token"].s();
        // timeFrame = 7 || 30 || 365 ie w, m, y
        // {TBD}
   });

    // Agenda
        // GET Agenda (Also accessible to employee jwt)
    CROW_ROUTE(app, "/agenda").methods("GET"_method)
        ([](const crow::request& req) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string token = requestBody["token"].s();
        // return list of agenda for the day (24h)
    });

        // POST agenda (only for admin)
    CROW_ROUTE(app, "/agenda").methods("POST"_method)
        ([](const crow::request& req) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string token = requestBody["token"].s();
        // {message}
    });

    // Attendance Route
        // GET attendance details
    CROW_ROUTE(app, "/attendance/<int>/<int>").methods("GET"_method)
        ([] (crow::request& req, int employeeID, int timeFrame) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string token = requestBody["token"].s();
        // [{date, logInTime, logOutTime}]    
        });
   

    // Addition routes

    CROW_ROUTE(app, "/add/inventory").methods("POST"_method)
        ([] (const crow::request& req) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string token = requestBody["token"].s();
        // {productID, message}
     });

    CROW_ROUTE(app, "/add/employee").methods("POST"_method)
        ([](const crow::request& req) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string token = requestBody["token"].s();
        // {employeeID, username, password}
    });

    CROW_ROUTE(app, "/add/discount").methods("POST"_method)
        ([](const crow::request& req) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string token = requestBody["token"].s();
        // {message}
    });

    // Removal Routes

    CROW_ROUTE(app, "/remove/employee").methods("DELETE"_method)
        ([](const crow::request& req) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string token = requestBody["token"].s();
        // {message}
    });

    CROW_ROUTE(app, "/remove/discount").methods("DELETE"_method)
        ([](const crow::request& req) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string token = requestBody["token"].s();
        // {message}
    });


    app.port(18080)
      .server_name("VittiWebServer")
      .multithreaded()
      .run();
}
