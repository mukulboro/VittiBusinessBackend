#pragma once
#include "sqlite3.h"
#include"customDataTypes.h"
#include <ctime>

/*----------------LOGIN VARIABLES--------------------*/

LoginDetails loginData;
int loginCallback(void* literalVoid, int noOfEntries, char** values, char** keys) {
    LoginDetails temp;

    temp.employeeID = values[0];
    temp.name = values[1];
    temp.post = values[2];
    temp.role = values[3];
    temp.username = values[4];
    temp.password = values[5];

    loginData = temp;

    return 0;
}

/*----------------LOGIN VARIABLES--------------------*/

/*----------------ATTENDANCE VARIABLES--------------------*/

AttendanceDetails listOfAttendance[MAX];
int attendanceCounter = 0;

int attendanceCallback(void* literalVoid, int noOfColumns, char** values, char** keys) {

    listOfAttendance[attendanceCounter].employeeID = values[0]; // ID
    listOfAttendance[attendanceCounter].employeeName = values[1]; // Name
    listOfAttendance[attendanceCounter].datetime = values[2]; // datetime

    attendanceCounter++;
    return 0;
}

/*----------------ATTENDANCE VARIABLES--------------------*/

/*----------------PRODUCT VARIABLES--------------------*/
ProductDetails productDetail;
int productCallback(void* literalVoid, int noOfEntries, char** values, char** keys) {
    productDetail.productID = values[0];
    productDetail.productName = values[1];
    productDetail.minPrice = values[2];
    productDetail.maxPrice = values[3];
    productDetail.price = values[4];
    productDetail.stock = values[5];
    return 0;
}
/*----------------ORDER VARIABLES--------------------*/

OrderDetails orderDetail;
int orderCallback(void* literalVoid, int noOfEntries, char** values, char** keys) {
    orderDetail.orderID = values[0];
    orderDetail.productID = values[1];
    orderDetail.customerName = values[2];
    orderDetail.customerContact = values[3];
    orderDetail.customerAddress = values[4];
    orderDetail.price = values[5];
    orderDetail.paymentMode = values[6];
    return 0;
}
/*----------------ORDER VARIABLES--------------------*/


OrderDetails listOfSales[MAX];
int salesCounter = 0;

int salesCallback(void* literalVoid, int noOfColumns, char** values, char** keys) {

    listOfSales[salesCounter].price = values[5];

    salesCounter++;
    return 0;
}


/*----------------INVENTORY VARIABLES--------------------*/

ProductDetails listOfProducts[MAX];
int productCounter = 0;

int inventoryCallback(void* literalVoid, int noOfColumns, char** values, char** keys) {
    listOfProducts[productCounter].productID = values[0];
    listOfProducts[productCounter].productName = values[1];
    listOfProducts[productCounter].minPrice = values[2];
    listOfProducts[productCounter].maxPrice = values[3];
    listOfProducts[productCounter].price = values[4];
    listOfProducts[productCounter].stock = values[5];

    productCounter++;
    return 0;
}

/*----------------INVENTORY VARIABLES--------------------*/

/*----------------ALL ORDERS VARIABLES--------------------*/

OrderDetails listOfOrders[MAX];
int orderCounter = 0;

int allOrdersCallback(void* literalVoid, int noOfColumns, char** values, char** keys) {
    listOfOrders[orderCounter].orderID = values[0];
    listOfOrders[orderCounter].productID = values[1];
    listOfOrders[orderCounter].customerName = values[2];
    listOfOrders[orderCounter].customerContact = values[3];
    listOfOrders[orderCounter].customerAddress = values[4];
    listOfOrders[orderCounter].price = values[5];
    listOfOrders[orderCounter].paymentMode = values[6];

    orderCounter++;
    return 0;
}

/*----------------ALL ORDERS VARIABLES--------------------*/



/*----------------DISCOUNT VARIABLES--------------------*/

DiscountDetails listOfDiscounts[MAX];
int discountCounter = 0;

int discountCallback(void* literalVoid, int noOfColumns, char** values, char** keys) {
    listOfDiscounts[discountCounter].code = values[0];
    listOfDiscounts[discountCounter].amount = values[1];
    listOfDiscounts[discountCounter].validity = values[2];

    discountCounter++;
    return 0;
}
/*----------------DISCOUNT VARIABLES--------------------*/

/*----------------AGENDA VARIABLES--------------------*/
AgendaDetails listOfAgenda[MAX];
int agendaCounter = 0;

int agendaCallback(void* literalVoid, int noOfColumns, char** values, char** keys) {
    listOfAgenda[agendaCounter].agenda = values[0];
    listOfAgenda[agendaCounter].postedDate = values[1];
    agendaCounter++;
    return 0;
}
/*----------------AGENDA VARIABLES--------------------*/



int defaultCallback(void* data, int argc, char** argv, char** azColName) {
    // Return successful
    return 0;
}



class Database {
    private:
        int isDbError = 0;
        std::string sql;
        sqlite3* db;
        char* zErrMsg = 0;
        std::string dbName;

    public:
        Database(std::string dName){
            dbName = dName;
        }


        int openDatabase() {
            isDbError = sqlite3_open(dbName.c_str(), &db);
            return isDbError;
        }

        int initializeTables() {
            sql = "CREATE TABLE IF NOT EXISTS Employee (EmployeeID int,name varchar(256),post varchar(256),role varchar(10),username varchar(256),password varchar(256));";
            isDbError = sqlite3_exec(db, sql.c_str(), defaultCallback, 0, &zErrMsg);

            if (isDbError) return 0;

            sql = "CREATE TABLE IF NOT EXISTS Product(productID int, productName varchar(256),minPrice int,maxPrice int,price int,stock int);";
            isDbError = sqlite3_exec(db, sql.c_str(), defaultCallback, 0, &zErrMsg);
            
            if (isDbError) return 0;


            sql = "CREATE TABLE IF NOT EXISTS Orders(orderID int,productID int,customerName varchar(256),customerContact int,customerAddress varchar(256),price int, paymentMode varchar(256));";
            isDbError = sqlite3_exec(db, sql.c_str(), defaultCallback, 0, &zErrMsg);

            if (isDbError) return 0;


            sql = "CREATE TABLE IF NOT EXISTS Agenda(agenda varchar(256),postedDate DATE);";
            isDbError = sqlite3_exec(db, sql.c_str(), defaultCallback, 0, &zErrMsg);

            if (isDbError) return 0;


            sql = "CREATE TABLE IF NOT EXISTS Discount(code varchar(50),amount int,validity DATE);";
            isDbError = sqlite3_exec(db, sql.c_str(), defaultCallback, 0, &zErrMsg);

            if (isDbError) return 0;


            sql = "CREATE TABLE IF NOT EXISTS Attendance(employeeID int,employeeName varchar(256),dateTime DATETIME);";
            isDbError = sqlite3_exec(db, sql.c_str(), defaultCallback, 0, &zErrMsg);

            if (isDbError) return 0;
            
            return 1;
        }
        /*----------------LOGIN METHODS START--------------------*/
        LoginDetails login(std::string username, std::string password) {

            LoginDetails toReturn;

            sql = "SELECT * FROM Employee WHERE username='"+username+"';";
            isDbError = sqlite3_exec(db, sql.c_str(), loginCallback, 0, &zErrMsg);

            toReturn = loginData;

            return toReturn;
        }
        /*----------------LOGIN METHODS END--------------------*/

        /*----------------ATTENDANCE METHODS START--------------------*/
        void doAttendance(std::string id, std::string name, std::string date) {
            sql = "INSERT INTO Attendance(employeeID,employeeName,dateTime) VALUES('" + id + "','" + name+"','" + date + "');";
            isDbError = sqlite3_exec(db, sql.c_str(), defaultCallback, 0, &zErrMsg);

        }

        AttendanceDetails* getAttendance(int id) {
            sql = "SELECT * FROM ATTENDANCE WHERE employeeID='"+std::to_string(id)+"';";
            isDbError = sqlite3_exec(db, sql.c_str(), attendanceCallback, 0, &zErrMsg);
            return listOfAttendance;
        }

        int numberOfAttendance() {
            int temp = attendanceCounter;
            attendanceCounter = 0;
            return temp;
        }

        /*----------------ATTENDANCE METHODS END--------------------*/

        /*----------------PRODUCT METHODS START--------------------*/
        ProductDetails getProductDetail(int id) {
            ProductDetails toReturn;
            sql = "SELECT * FROM Product WHERE productID='" + std::to_string(id) + "';";
            isDbError = sqlite3_exec(db, sql.c_str(), productCallback, 0, &zErrMsg);
            toReturn = productDetail;
            return toReturn;
        }
        /*----------------PRODUCT METHODS END--------------------*/

        /*----------------ORDER METHODS START--------------------*/
        OrderDetails getOrderDetail (int id) {
            OrderDetails toReturn;
            sql = "SELECT * FROM Orders WHERE orderID='" + std::to_string(id) + "';";
            isDbError = sqlite3_exec(db, sql.c_str(), orderCallback, 0, &zErrMsg);
            toReturn = orderDetail;
            return toReturn;
        }

        void addOrder(OrderDetails detail) {
            std::string id = detail.orderID;
            std::string product = detail.productID;
            std::string name = detail.customerName;
            std::string contact = detail.customerContact;
            std::string address = detail.customerAddress;
            std::string price = detail.price;
            std::string paymentMode = detail.paymentMode;

            sql = "INSERT INTO Orders VALUES('" + id + "','" + product + "','" + name + "','" + contact + "','" + address + "','" + price + "','" + paymentMode + "')";
            isDbError = sqlite3_exec(db, sql.c_str(), defaultCallback, 0, &zErrMsg);

        }

        void deleteOrder(int id) {
            sql = "DELETE FROM Orders WHERE orderID='" + std::to_string(id) + "';";
            isDbError = sqlite3_exec(db, sql.c_str(), defaultCallback, 0, &zErrMsg);
        }
        /*----------------ORDER METHODS END--------------------*/
        
        /*----------------SALES METHODS START--------------------*/

        int getAllSales() {
            int amount = 0;
            sql = "SELECT * FROM Orders";
            isDbError = sqlite3_exec(db, sql.c_str(), salesCallback, 0, &zErrMsg);

            for (int i = 0; i < salesCounter; i++) {
                amount += std::stoi(listOfSales[i].price);
            }

            return amount;
        }

        int numberOfSales() {
            int temp = salesCounter;
            salesCounter = 0;
            return temp;
        }

        /*----------------SALES METHODS END--------------------*/


        /*----------------AGENDA METHODS START--------------------*/
        void addAgenda(std::string agenda) {
            std::string datetime = getDateTime();
            sql = "INSERT INTO Agenda VALUES('"+agenda+"','"+datetime+"');";
            isDbError = sqlite3_exec(db, sql.c_str(), defaultCallback, 0, &zErrMsg);
        }

        AgendaDetails* getAgenda() {
            AgendaDetails* toReturn;
            sql = "SELECT * FROM Agenda";
            isDbError = sqlite3_exec(db, sql.c_str(), agendaCallback, 0, &zErrMsg);
            toReturn = listOfAgenda;
            return toReturn;
        }

        int numberOfAgenda() {
            int temp = agendaCounter;
            agendaCounter = 0;
            return temp;
        }
        /*----------------AGENDA METHODS END--------------------*/

        /*----------------INVENTORY METHODS START--------------------*/
        ProductDetails* getInventory() {
            ProductDetails* toReturn;
            sql = "SELECT * FROM Product;";
            isDbError = sqlite3_exec(db, sql.c_str(), inventoryCallback, 0, &zErrMsg);
            toReturn = listOfProducts;
            return toReturn;
        }

        int numberOfInventory() {
            int temp = productCounter;
            productCounter = 0;
            return temp;
        }

        void deleteInventory(int id) {
            sql = "DELETE FROM Product WHERE productID='" + std::to_string(id) + "';";
            isDbError = sqlite3_exec(db, sql.c_str(), defaultCallback, 0, &zErrMsg);
        }

        void updateInventory(int ID, std::string price, std::string stock) {
            sql = "UPDATE Product SET price='" + price + "', stock='" + stock + "' WHERE productID='" + std::to_string(ID) + "';";
            isDbError = sqlite3_exec(db, sql.c_str(), defaultCallback, 0, &zErrMsg);

        }

        void addInventory(std::string productID, std::string name, std::string minPrice, 
            std::string maxPrice, std::string price, std::string stock) {
            sql = "INSERT INTO Product VALUES('" + productID + "','" + name + "','" + minPrice + "','" + maxPrice + "','" + price + "','" + stock + "');";
            isDbError = sqlite3_exec(db, sql.c_str(), defaultCallback, 0, &zErrMsg);
        
        }
        /*----------------INVENTORY METHODS END--------------------*/

        /*----------------REVISION METHODS START--------------------*/

        OrderDetails* getAllOrders() {
            OrderDetails* toReturn;
            sql = "SELECT * FROM Orders;";
            isDbError = sqlite3_exec(db, sql.c_str(), allOrdersCallback, 0, &zErrMsg);
            toReturn = listOfOrders;
            return toReturn;
        }

        int numberOfOrder() {
            int temp = orderCounter;
            orderCounter = 0;
            return temp;
        }

        /*----------------REVISION METHODS END--------------------*/


        /*----------------DISCOUNT METHODS START--------------------*/
        DiscountDetails* getDiscountCodes() {
            DiscountDetails* toReturn;
            sql = "SELECT * FROM Discount;";
            isDbError = sqlite3_exec(db, sql.c_str(), discountCallback, 0, &zErrMsg);
            toReturn = listOfDiscounts;
            return toReturn;
           
        }

        int numberOfDiscount() {
            int temp = discountCounter;
            discountCounter = 0;
            return temp;
        }

        void deleteDiscount(std::string code ) {
            sql = "DELETE FROM Discount WHERE code='" + code + "';";
            isDbError = sqlite3_exec(db, sql.c_str(), defaultCallback, 0, &zErrMsg);
        }

        void addDiscountCode(std::string code, std::string amount, std::string validity) {
            sql = "INSERT INTO Discount VALUES('"+code+"','"+amount+"','"+validity+"');";
            isDbError = sqlite3_exec(db, sql.c_str(), defaultCallback, 0, &zErrMsg);
        }
        /*----------------DISCOUNT METHODS END--------------------*/

        /*----------------EMPLOYEE METHODS START--------------------*/
        void deleteEmployee(std::string id) {
            sql = "DELETE FROM Employee WHERE EmployeeID='" + id + "';";
            isDbError = sqlite3_exec(db, sql.c_str(), defaultCallback, 0, &zErrMsg);
        }

        void addEmployee(std::string id, std::string name, std::string post, std::string role, 
            std::string username, std::string password) {
            sql = "INSERT INTO Employee VALUES('"+id+"','"+name+"','"+post+"','"+role+"','"+username+"','"+password+"');";
            isDbError = sqlite3_exec(db, sql.c_str(), defaultCallback, 0, &zErrMsg);
        }
        /*----------------EMPLOYEE METHODS END--------------------*/

        void close(){
            sqlite3_close(db);
        }
};