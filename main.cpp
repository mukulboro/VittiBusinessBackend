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
        //ROUTE COMPLETED
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
        ([&](const crow::request& req, int inventoryID) {
        // ROUTE COMPLETED
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});

        if (!inventoryID) {
            std::vector<crow::json::wvalue> list;
            ProductDetails* listOfValues;
            int numberOfValues;
            listOfValues = database.getInventory();
            numberOfValues = database.numberOfInventory();

           
            for (int i = 0; i < numberOfValues; i++) {
                response["productID"] = listOfValues[i].productID;
                response["productName"] = listOfValues[i].productName;
                response["minPrice"] = listOfValues[i].minPrice;
                response["maxPrice"] = listOfValues[i].maxPrice;
                response["price"] = listOfValues[i].price;
                response["stock"] = listOfValues[i].stock;

                list.push_back(response);
            }

            crow::json::wvalue listResponse(crow::json::wvalue::list({ list }));
            return listResponse;
        }
        else {
            ProductDetails detail = database.getProductDetail(inventoryID);
            response["productID"] = detail.productID;
            response["productName"] = detail.productName;
            response["minPrice"] = detail.minPrice;
            response["maxPrice"] = detail.maxPrice;
            response["price"] = detail.price;
            response["stock"] = detail.stock;
            return response;
        }
        });

    // Update Inventory
        // Using POST instead of PATCH because [REDACTED] it
    CROW_ROUTE(app, "/inventory/<int>").methods("POST"_method)
        ([&database](const crow::request& req, int inventoryID) {
        //ROUTE COMPLETED
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string quantity = requestBody["quantity"].s();
        std::string price = requestBody["price"].s();
        database.updateInventory(inventoryID, price, quantity);
        response["message"] = "Updated Product";
        return response;
    });

    // DETELE Inventory
    CROW_ROUTE(app, "/inventory/<int>").methods("DELETE"_method)
        ([&database](const crow::request& req, int inventoryID) {
        //ROUTE COMPLETED
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        database.deleteInventory(inventoryID);
        response["message"] = "Deleted product.";
        return response;
       });

    //Discount Code
        // GET Discount 
    CROW_ROUTE(app, "/discount").methods("GET"_method)
        ([&database](const crow::request& req) {
        //ROUTE COMPLETED
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::vector<crow::json::wvalue> list;
        DiscountDetails* listOfValues;
        int numberOfValues;
        listOfValues = database.getDiscountCodes();
        numberOfValues = database.numberOfDiscount();

        for (int i = 0; i < numberOfValues; i++) {
            response["code"] = listOfValues[i].code;
            response["percentage"] = listOfValues[i].amount;
            response["validity"] = listOfValues[i].validity;
            list.push_back(response);
        }

        crow::json::wvalue listResponse(crow::json::wvalue::list({ list }));
        return listResponse;
        
         });

    // Sales Data
       // GET Sales Data

    CROW_ROUTE(app, "/sales").methods("GET"_method)
        ([&database](const crow::request& req) {
        //ROUTE COMPLETED
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        int totalSales = database.getAllSales();
        int number = database.numberOfSales();
        response["totalSales"] = totalSales;
        response["numberOfSales"] = number;
        response["averageSalesAmount"] = totalSales / number;
        return response;
      });

    // Agenda
        // GET Agenda
    CROW_ROUTE(app, "/agenda").methods("GET"_method)
        ([&database](const crow::request& req) {
        //ROUTE COMPLETED
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::vector<crow::json::wvalue> list;
        AgendaDetails* listOfValues;
        int numberOfValues;
        listOfValues = database.getAgenda();
        numberOfValues = database.numberOfAgenda();

        for (int i = 0; i < numberOfValues; i++) {
            response["agenda"] = listOfValues[i].agenda;
            response["postedDate"] = listOfValues[i].postedDate;
            list.push_back(response);
        }

        crow::json::wvalue listResponse(crow::json::wvalue::list({ list }));
        return listResponse;
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
        
        crow::json::wvalue listResponse(crow::json::wvalue::list({ list }));
        return listResponse;
        });

    // Addition routes

    CROW_ROUTE(app, "/add/inventory").methods("POST"_method)
        ([&database](const crow::request& req) {
        // ROUTE COMPLETED
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string maxPrice = requestBody["maxPrice"].s();
        std::string productName = requestBody["name"].s();
        std::string minPrice = requestBody["minPrice"].s();
        std::string sellingPrice = requestBody["price"].s();
        std::string stock = requestBody["stock"].s();
        std::string productID = std::to_string(time(0));
        database.addInventory(productID, productName, minPrice, maxPrice, sellingPrice, stock); 
        response["productID"] = productID;
        response["message"] = "New Product Added";
        return response;
            });

    CROW_ROUTE(app, "/add/employee").methods("POST"_method)
        ([&database](const crow::request& req) {
        //ROUTE COMPLETED
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string name = requestBody["name"].s();
        std::string post = requestBody["post"].s();
        std::string role = requestBody["role"].s();
        std::string username = requestBody["username"].s();
        std::string password = requestBody["password"].s();
        std::string empId = std::to_string(time(0));
        database.addEmployee(empId, name, post, role, username, password);
        response["employeeID"] = empId;
        response["username"] = username;
        response["password"] = password;
        response["message"] = "New Employee Added";
        return response;
        });

    CROW_ROUTE(app, "/add/discount").methods("POST"_method)
        ([&database](const crow::request& req) {
        //ROUTE COMPLETED
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string code = requestBody["code"].s();
        std::string amount = requestBody["amount"].s();
        std::string validity = requestBody["validity"].s();
        database.addDiscountCode(code, amount, validity);
        response["message"] = "Added new discount code";
        return response;
            });

    // Removal Routes

    CROW_ROUTE(app, "/remove/employee").methods("DELETE"_method)
        ([&database](const crow::request& req) {
        //ROUTE COMPLETED
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string employeeID = requestBody["employeeID"].s();
        database.deleteEmployee(employeeID);
        response["message"] = "Deleted given employee";
        return response;
            });

    CROW_ROUTE(app, "/remove/discount").methods("DELETE"_method)
        ([&database](const crow::request& req) {
        // ROUTE COMPLETED
        auto requestBody = crow::json::load(req.body);
        crow::json::wvalue response({});
        std::string code = requestBody["code"].s();
        database.deleteDiscount(code);
        response["message"] = "Deleted Given Code";
        return response;
            });

    
    app.port(PORT)
      .server_name("VittiWebServer")
      .multithreaded()
      .run();

    database.close();
    return 0;
}
