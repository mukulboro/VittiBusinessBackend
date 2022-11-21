#include "crow_all.h"
#include "../WebServer/WebServer/database.h"
#include "../WebServer/WebServer/customDataTypes.h"
#include "sqlite3.h"
#define PORT 6969


int main()
{

    int isDbError = 0;
    crow::SimpleApp app;
    Database database("vittiDB.db");

    CROW_LOG_INFO << ":::Server is running on port " << PORT <<":" << getDateTime()<<":::";

    // Opening Database 

    isDbError = database.openDatabase();

    if (isDbError) {
        CROW_LOG_WARNING << ":::DB Open Error:::";
    }else{
        CROW_LOG_INFO << ":::Database opened successfully:::";
    }

    // Initializing database tables

    if (database.initializeTables()) {
        CROW_LOG_INFO << ":::DB Tables initialized:::";
    }
    else {
        CROW_LOG_INFO << ":::Error initializing tables:::";
    }

    CROW_ROUTE(app, "/health").methods("GET"_method)
        ([]() {
        // ROUTE COMPLETED
        CROW_LOG_INFO << ":::Server health checked:::";
        return "true";
    });
   
    //--------------LOGIN ROUTE--------------------------

    CROW_ROUTE(app, "/login").methods("POST"_method)
    ([&database](const crow::request& req){
        // ROUTE COMPLETED
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string userName, password;
        bool isValidUser = false;
        userName = requestBody["username"].s();
        password = requestBody["password"].s();

        LoginDetails dbData = database.login(userName, password);

        if (password == dbData.password) isValidUser = true;

        if (isValidUser) {
            database.doAttendance(dbData.employeeID, dbData.name, getDateTime());
            response["isValidUser"] = isValidUser;
            response["userName"] = dbData.username;
            response["employeeID"] = dbData.employeeID;
            response["post"] = dbData.post;
            response["role"] = dbData.role;
            return response;
        }
        else {
            response["isValidUser"] = isValidUser;
            return response;
        }
    });

    //------------------EMPLOYEE ACTIVITY ROUTES-------------------

    //Product Details

    CROW_ROUTE(app, "/product/<int>").methods("GET"_method)
     ([&database](const crow::request& req, int productID) {
        // ROUTE COMPLETED
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        ProductDetails detail = database.getProductDetail(productID);
        response["productID"] = detail.productID;
        response["productName"] = detail.productName;
        response["minPrice"] = detail.minPrice;
        response["maxPrice"] = detail.maxPrice;
        response["price"] = detail.price;
        response["stock"] = detail.stock;
        return response;
     }); 


    // Order Details

        // GET order details
    CROW_ROUTE(app, "/orders/<int>").methods("GET"_method)
        ([&database](const crow::request& req, int orderID) {
        // ROUTE COMPLETED
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        OrderDetails detail = database.getOrderDetail(orderID);

        response["orderID"] = detail.orderID;
        response["productID"] = detail.productID;
        response["customerName"] = detail.customerName;
        response["customerContact"] = detail.customerContact;
        response["customerAddress"] = detail.customerAddress;
        response["price"] = detail.price;

        return response;
     });


    //ADD Orders (post)
    CROW_ROUTE(app, "/orders").methods("POST"_method)
        ([&database](const crow::request& req) {
        // ROUTE COMPLETED
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        OrderDetails toSend;
        std::string productID = requestBody["product"].s();
        std::string customerName = requestBody["customer"].s();
        std::string customerContact = requestBody["contact"].s();
        std::string customerAddress = requestBody["address"].s();
        std::string price = requestBody["price"].s();
        toSend.orderID = std::to_string(time(0));
        toSend.productID = productID;
        toSend.customerName = customerName;
        toSend.customerAddress = customerAddress;
        toSend.customerContact = customerContact;
        toSend.price = price;
        database.addOrder(toSend);
        response["orderID"] = toSend.orderID;
        response["message"] = "Added Order";
        return response;
     });


    //REMOVE Orders (delete)
    CROW_ROUTE(app, "/orders/<int>").methods("DELETE"_method)
        ([&database](const crow::request& req, int orderID) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        database.deleteOrder(orderID);
        response["message"] = "Order Deleted";
        return response;
   });


    //------------------ADMINACTIVITY ROUTES-------------------

    // Inventory Details
        // GET Inventory
    CROW_ROUTE(app, "/inventory/<int>").methods("GET"_method)
        ([](const crow::request& req, int inventoryID) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        // inventoryID is synonymous with productID
        // return whole inventory if inventoryID == 0
        response["inventoryID"] = inventoryID;
        return response;
        });

    // Update Inventory
        // Using POST instead of PATCH because fuck it
    CROW_ROUTE(app, "/inventory/<int>").methods("POST"_method)
        ([](const crow::request& req, int inventoryID) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string quantity = requestBody["quantity"].s();
        // update inventory    
        response["inventoryID"] = inventoryID;
        response["quantity"] = quantity;
        return response;
    });

    // DETELE Inventory
    CROW_ROUTE(app, "/inventory/<int>").methods("DELETE"_method)
        ([](const crow::request& req, int inventoryID) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        // delete inventory
        response["inventoryID"] = inventoryID;
        return response;
       });

    //Discount Code
        // GET Discount (+ removal route below)
    CROW_ROUTE(app, "/discount").methods("GET"_method)
        ([](const crow::request& req) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        // {code, validity, percentage}
        return response;
         });

    // Sales Data
       // GET Sales Data

    CROW_ROUTE(app, "/sales/<int>").methods("GET"_method)
        ([](const crow::request& req, int timeFrame) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        // timeFrame = 7 || 30 || 365 ie w, m, y
        // {TBD}
        response["timeFrame"] = timeFrame;
        return response;
      });

    // Agenda
        // GET Agenda
    CROW_ROUTE(app, "/agenda").methods("GET"_method)
        ([](const crow::request& req) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        // return list of agenda for the day (24h)
        return response;
        });

    // POST agenda
    CROW_ROUTE(app, "/agenda").methods("POST"_method)
        ([&database](const crow::request& req) {
        // ROUTE COMPLETED

        std::string agenda;
        auto requestBody = crow::json::load(req.body);
        agenda = requestBody["agenda"].s();
        database.addAgenda(agenda);
        crow::json::wvalue response({});
        response["message"] = "Added Agenda";

        return response;
        });

    // Attendance Route
        // GET attendance details
    CROW_ROUTE(app, "/attendance/<int>").methods("GET"_method)
        ([&](const crow::request& req, int employeeID) {
        // ROUTE COMPLETED
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response;
        std::vector<crow::json::wvalue> list;
        AttendanceDetails* listOfValues;
        int numberOfValues;
        listOfValues = database.getAttendance(employeeID);
        numberOfValues = database.numberOfAttendance();

        for (int i = 0; i < numberOfValues; i++) {
            response["employeeID"] = listOfValues[i].employeeID;
            response["employeeName"] = listOfValues[i].employeeName;
            response["datetime"] = listOfValues[i].datetime;

            list.push_back(response);
        }
        
        //TODO: RETURN LIST JSON
        crow::json::wvalue listResponse(crow::json::wvalue::list({ list }));
        return listResponse;
        });

    // Addition routes

    CROW_ROUTE(app, "/add/inventory").methods("POST"_method)
        ([](const crow::request& req) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string maxPrice = requestBody["maxPrice"].s();
        std::string minPrice = requestBody["minPrice"].s();
        std::string sellingPrice = requestBody["price"].s();
        std::string stock = requestBody["stock"].s();
        // {productID, message}
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
        std::string name = requestBody["name"].s();
        std::string post = requestBody["post"].s();
        std::string role = requestBody["role"].s();
        std::string salary = requestBody["salary"].s();
        // {employeeID, username, password}
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
        std::string code = requestBody["code"].s();
        std::string amount = requestBody["amount"].s();
        std::string validity = requestBody["validity"].s();
        // {message}
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
        std::string employeeID = requestBody["employeeID"].s();
        // {message}
        response["employeeID"] = employeeID;
        return response;
            });

    CROW_ROUTE(app, "/remove/discount").methods("DELETE"_method)
        ([](const crow::request& req) {
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string code = requestBody["code"].s();
        // {message}
        response["code"] = code;
        return response;
            });

    
    app.port(PORT)
      .server_name("VittiWebServer")
      .multithreaded()
      .run();

    database.close();
    return 0;
}
