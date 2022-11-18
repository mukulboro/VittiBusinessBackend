#include "crow_all.h"
#include "sqlite3.h"
#define PORT 6969

int main()
{
    int isDbError = 0;
    CROW_LOG_INFO << "Server is running on port "<<PORT;
    // Opening Database 
    sqlite3* db;

    isDbError = sqlite3_open("vittiDB.db", &db);

    if (isDbError) {
        CROW_LOG_WARNING << "DB Open Error: " << sqlite3_errmsg(db);
    }else{
        CROW_LOG_INFO << "Database opened successfully";
    }

    crow::SimpleApp app;

    CROW_ROUTE(app, "/health").methods("GET"_method)
        ([]() {
        CROW_LOG_INFO << "Server health checked.";
        return "true";
    });
   
    //--------------LOGIN ROUTE--------------------------

    CROW_ROUTE(app, "/login").methods("POST"_method)
    ([](const crow::request& req){
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string userName, password;
        userName = requestBody["username"].s();
        password = requestBody["password"].s();
        // TODO: Do attendance
        // TODO: Return JWT
        response["userName"] = userName;
        response["password"] = password;
        // {jwt, role}
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
        response["token"] = token;
        response["orderID"] = orderID;
        return response;
     });


    //ADD Orders (post)
    CROW_ROUTE(app, "/orders/<int>").methods("POST"_method)
        ([](const crow::request& req, int orderID) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string token = requestBody["token"].s();
        std::string productID = requestBody["product"].s();
        std::string customerName = requestBody["customer"].s();
        std::string customerContact = requestBody["contact"].s();
        std::string customerAddress = requestBody["address"].s();
        std::string price = requestBody["price"].s();
        // {orderID, message}
        response["token"] = token;
        response["orderID"] = orderID;
        response["productID"] = productID;
        response["customerName"] = customerName;
        response["customerContact"] = customerContact;
        response["customerAddress"] = customerAddress;
        response["price"] = price;
        return response;
     });


    //REMOVE Orders (delete)
    CROW_ROUTE(app, "/orders/<int>").methods("DELETE"_method)
        ([](const crow::request& req, int orderID) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string token = requestBody["token"].s();
        // {message}
        response["token"] = token;
        response["orderID"] = orderID;
        return response;
   });


    //------------------ADMINACTIVITY ROUTES-------------------

    // Inventory Details
        // GET Inventory
    CROW_ROUTE(app, "/inventory/<int>").methods("GET"_method)
        ([](const crow::request& req, int inventoryID) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string token = requestBody["token"].s();
        // inventoryID is synonymous with productID
        // return whole inventory if inventoryID == 0
        response["token"] = token;
        response["inventoryID"] = inventoryID;
        return response;
        });

    // Update Inventory
        // Using POST instead of PATCH because fuck it
    CROW_ROUTE(app, "/inventory/<int>").methods("POST"_method)
        ([](const crow::request& req, int inventoryID) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string token = requestBody["token"].s();
        std::string quantity = requestBody["quantity"].s();
        // update inventory    
        response["token"] = token;
        response["inventoryID"] = inventoryID;
        response["quantity"] = quantity;
        return response;
    });

    // DETELE Inventory
    CROW_ROUTE(app, "/inventory/<int>").methods("DELETE"_method)
        ([](const crow::request& req, int inventoryID) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string token = requestBody["token"].s();
        // delete inventory
        response["token"] = token;
        response["inventoryID"] = inventoryID;
        return response;
       });

    //Discount Code
        // GET Discount (+ removal route below)
    CROW_ROUTE(app, "/discount").methods("GET"_method)
        ([](const crow::request& req) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string token = requestBody["token"].s();
        // {code, validity, percentage}
        response["token"] = token;
        return response;
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
        response["token"] = token;
        response["timeFrame"] = timeFrame;
        return response;
      });

    // Agenda
        // GET Agenda (Also accessible to employee jwt)
    CROW_ROUTE(app, "/agenda").methods("GET"_method)
        ([](const crow::request& req) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string token = requestBody["token"].s();
        // return list of agenda for the day (24h)
        response["token"] = token;
        return response;
        });

    // POST agenda (only for admin)
    CROW_ROUTE(app, "/agenda").methods("POST"_method)
        ([](const crow::request& req) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string token = requestBody["token"].s();
        // {message}
        response["token"] = token;
        return response;
            });

    // Attendance Route
        // GET attendance details
    CROW_ROUTE(app, "/attendance/<int>/<int>").methods("GET"_method)
        ([](const crow::request& req, int employeeID, int timeFrame) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string token = requestBody["token"].s();
        // {date, loginTime}
        response["token"] = token;
        response["employeeID"] = employeeID;
        response["timeFrame"] = timeFrame;
        return response;
            });

    // Addition routes

    CROW_ROUTE(app, "/add/inventory").methods("POST"_method)
        ([](const crow::request& req) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string token = requestBody["token"].s();
        std::string maxPrice = requestBody["maxPrice"].s();
        std::string minPrice = requestBody["minPrice"].s();
        std::string sellingPrice = requestBody["price"].s();
        std::string stock = requestBody["stock"].s();
        // {productID, message}
        response["token"] = token;
        response["maxPrice"] = maxPrice;
        response["minPrice"] = minPrice;
        response["sellingPrice"] = sellingPrice;
        response["stock"] = stock;
        return response;
            });

    CROW_ROUTE(app, "/add/employee").methods("POST"_method)
        ([](const crow::request& req) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string token = requestBody["token"].s();
        std::string name = requestBody["name"].s();
        std::string post = requestBody["post"].s();
        std::string role = requestBody["role"].s();
        std::string salary = requestBody["salary"].s();
        // {employeeID, username, password}
        response["token"] = token;
        response["name"] = name;
        response["post"] = post;
        response["role"] = role;
        response["salary"] = salary;
        return response;
            });

    CROW_ROUTE(app, "/add/discount").methods("POST"_method)
        ([](const crow::request& req) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string token = requestBody["token"].s();
        std::string code = requestBody["code"].s();
        std::string amount = requestBody["amount"].s();
        std::string validity = requestBody["validity"].s();
        // {message}
        response["token"] = token;
        response["code"] = code;
        response["amount"] = amount;
        response["validity"] = validity;
        return response;
            });

    // Removal Routes

    CROW_ROUTE(app, "/remove/employee").methods("DELETE"_method)
        ([](const crow::request& req) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string token = requestBody["token"].s();
        std::string employeeID = requestBody["employeeID"].s();
        // {message}
        response["token"] = token;
        response["employeeID"] = employeeID;
        return response;
            });

    CROW_ROUTE(app, "/remove/discount").methods("DELETE"_method)
        ([](const crow::request& req) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string token = requestBody["token"].s();
        std::string code = requestBody["code"].s();
        // {message}
        response["token"] = token;
        response["code"] = code;
        return response;
            });

    
    app.port(PORT)
      .server_name("VittiWebServer")
      .multithreaded()
      .run();
}
