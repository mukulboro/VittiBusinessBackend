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
    return 0;
}
/*----------------ORDER VARIABLES--------------------*/



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
        Database(std::string name){
            name = dbName;
        }


        int openDatabase() {
            isDbError = sqlite3_open("vittiDB.db", &db);
            return isDbError;
        }

        int initializeTables() {
            sql = "CREATE TABLE IF NOT EXISTS Employee (EmployeeID int,name varchar(256),post varchar(256),role varchar(10),username varchar(256),password varchar(256));";
            isDbError = sqlite3_exec(db, sql.c_str(), defaultCallback, 0, &zErrMsg);

            if (isDbError) return 0;

            sql = "CREATE TABLE IF NOT EXISTS Product(productID int, productName varchar(256),minPrice int,maxPrice int,price int,stock int);";
            isDbError = sqlite3_exec(db, sql.c_str(), defaultCallback, 0, &zErrMsg);
            
            if (isDbError) return 0;


            sql = "CREATE TABLE IF NOT EXISTS Orders(orderID int,productID int,customerName varchar(256),customerContact int,customerAddress varchar(256),price int);";
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

            sql = "INSERT INTO Orders VALUES('" + id + "','" + product + "','" + name + "','" + contact + "','" + address + "','" + price + "');";
            isDbError = sqlite3_exec(db, sql.c_str(), defaultCallback, 0, &zErrMsg);

        }

        void deleteOrder(int id) {
            sql = "DELETE FROM Orders WHERE orderID='" + std::to_string(id) + "';";
            CROW_LOG_INFO << sql;
            isDbError = sqlite3_exec(db, sql.c_str(), defaultCallback, 0, &zErrMsg);
        }
        /*----------------ORDER METHODS END--------------------*/

        /*----------------AGENDA METHODS START--------------------*/
        void addAgenda(std::string agenda) {
            std::string datetime = getDateTime();
            isDbError = sqlite3_exec(db, sql.c_str(), defaultCallback, 0, &zErrMsg);
        }
        /*----------------AGENDA METHODS END--------------------*/


        void close(){
            sqlite3_close(db);
        }


};