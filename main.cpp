#include "crow_all.h"

int main()
{
    crow::SimpleApp app;

    //--------------LOGIN ROUTE--------------------------

    CROW_ROUTE(app, "/login").methods("POST"_method)
    ([](const crow::request& req){
        std::string userName, password;
        auto requestBody = crow::json::load(req.body);
        userName = requestBody["username"].s();
        password = requestBody["password"].s();
        // TODO: Do attendance
        // TODO: Return JWT
        crow::json::wvalue x({ {"username", userName} });
        x["password"] = password;
        x["role"] = "admin";
        x["token"] = "123abc";
        return x;
    });

    //------------------EMPLOYEE ACTIVITY ROUTES-------------------

    //Product Details

    CROW_ROUTE(app, "/product/<int>")
     ([](int productID) {
        // TODO: Return Product Details
        crow::json::wvalue x({ {"productID", productID} });
        x["key2"] = "value2";
        return x;
     }); 

    // Order Details
    
    CROW_ROUTE(app, "/orders/<int>").methods("GET"_method)
     ([](int orderID) {
        // TODO: GET order details
        // TODO: ADD Orders (post)
        // TODO:  REMOVE Orders (delete)
        crow::json::wvalue x({ {"orderID", orderID} });
        x["key2"] = "value2";
        x["httpMethod"] = "get";
        return x;
     });

    CROW_ROUTE(app, "/orders/<int>").methods("POST"_method)
        ([](int orderID) {
        // TODO: GET order details
        // TODO: ADD Orders (post)
        // TODO:  REMOVE Orders (delete)
        crow::json::wvalue x({ {"orderID", orderID} });
        x["httpMethod"] = "post";
        return x;
            });


    app.port(18080)
      .server_name("VittiWebServer")
      .multithreaded()
      .run();
}
