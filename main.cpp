#include "crow_all.h"

int main()
{
    crow::SimpleApp app;

    //--------------LOGIN ROUTE--------------------------

    CROW_ROUTE(app, "/login").methods("POST"_method)
    ([](const crow::request& req){
        std::string userName, password, role, token;
        auto requestBody = crow::json::load(req.body);
        userName = requestBody["username"].s();
        password = requestBody["password"].s();
        // TODO: Do attendance
        // TODO: Return JWT
        crow::json::wvalue valid({});
        valid["token"] = "123abc";
        return valid;
    });

    //------------------EMPLOYEE ACTIVITY ROUTES-------------------

    //Product Details

    CROW_ROUTE(app, "/product/<int>")
     ([](const crow::request& req, int productID) {
        auto requestBody = crow::json::load(req.body);
        std::string token = requestBody["token"].s();
        // TODO: Return Product Details
        // {name, minPrice, maxPrice, price, stock}
        crow::json::wvalue x({ {"productID", productID} });
        return x;
     }); 

    // Order Details

        // GET order details
    CROW_ROUTE(app, "/orders/<int>").methods("GET"_method)
     ([](const crow::request& req, int orderID) {
        auto requestBody = crow::json::load(req.body);
        std::string token = requestBody["token"].s();
        // {product, customerName, customerContact, orderStatus}
        crow::json::wvalue x({ {"orderID", orderID} });
        return x;
     });
        //ADD Orders (post)
    CROW_ROUTE(app, "/orders/<int>").methods("POST"_method)
        ([](const crow::request& req) {
        auto requestBody = crow::json::load(req.body);
        std::string token = requestBody["token"].s();
        // {orderID, message}
        crow::json::wvalue x({ {"orderID", "orderID"} });
        return x;
     });
        //REMOVE Orders (delete)
    CROW_ROUTE(app, "/orders/<int>").methods("DELETE"_method)
        ([](const crow::request& req, int orderID) {
        auto requestBody = crow::json::load(req.body);
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
        // inventoryID is synonymous with productID
        // return whole inventory if inventoryID == 0

        });
        // Update Inventory
        // Using POST instead of PATCH because fuck it
    CROW_ROUTE(app, "/inventory/<int>").methods("POST"_method)
        ([] (const crow::request& req, int inventoryID) {
        // update inventory    
        });

        // DETELE Inventory
    CROW_ROUTE(app, "/inventory/<int>").methods("DELETE"_method)
        ([](const crow::request& req, int inventoryID) {
        // delete inventory    
        });

    // Addition routes

    CROW_ROUTE(app, "/add/inventory").methods("POST"_method)
        ([] (const crow::request& req) {
        // {productID, message}
     });

    CROW_ROUTE(app, "/add/employee").methods("POST"_method)
        ([](const crow::request& req) {
        // {employeeID, username, password}
    });

    CROW_ROUTE(app, "/add/discount").methods("POST"_method)
        ([](const crow::request& req) {
        // {message}
    });

    // Removal Routes








    app.port(18080)
      .server_name("VittiWebServer")
      .multithreaded()
      .run();
}
