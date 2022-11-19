#pragma once
#include "sqlite3.h"
#include"customDataTypes.h"

LoginDetails loginData;
int loginCounter = 0;

int defaultCallback(void* data, int argc, char** argv, char** azColName) {
    // Return successful
    return 0;
}

int loginCallback(void* literalVoid, int noOfEntries, char** values, char** keys) {
    LoginDetails temp;

    temp.employeeID = *values[0];
    temp.name = values[1];
    temp.post = values[2];
    temp.role = values[3];
    temp.username = values[4];
    temp.password = values[5];

    loginData = temp;
    
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

            sql = "CREATE TABLE IF NOT EXISTS Product(productID int,minPrice int,maxPrice int,price int,stock int);";
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

            //sql = "CREATE TABLE IF NOT EXISTS BIGPP(employeeID int,employeeName varchar(256),dateTime DATETIME);";
            //isDbError = sqlite3_exec(db, sql.c_str(), defaultCallback, 0, &zErrMsg);

            if (isDbError) return 0;
            
            return 1;
        }

        LoginDetails login(std::string username, std::string password) {

            LoginDetails toReturn;

            sql = "SELECT * FROM Employee WHERE username='"+username+"';";
            isDbError = sqlite3_exec(db, sql.c_str(), loginCallback, 0, &zErrMsg);

            toReturn = loginData;

            return toReturn;
        }

        void close(){
            sqlite3_close(db);
        }


};