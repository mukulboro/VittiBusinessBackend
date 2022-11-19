#pragma once
#include "sqlite3.h"

int serverCallback(void* data, int argc, char** argv, char** azColName) {


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
            isDbError = sqlite3_exec(db, sql.c_str(), serverCallback, 0, &zErrMsg);

            if (isDbError) return 0;

            sql = "CREATE TABLE IF NOT EXISTS Product(productID int,minPrice int,maxPrice int,price int,stock int);";
            isDbError = sqlite3_exec(db, sql.c_str(), serverCallback, 0, &zErrMsg);
            
            if (isDbError) return 0;


            sql = "CREATE TABLE IF NOT EXISTS Orders(orderID int,productID int,customerName varchar(256),customerContact int,customerAddress varchar(256),price int);";
            isDbError = sqlite3_exec(db, sql.c_str(), serverCallback, 0, &zErrMsg);

            if (isDbError) return 0;


            sql = "CREATE TABLE IF NOT EXISTS Agenda(agenda varchar(256),postedDate DATE);";
            isDbError = sqlite3_exec(db, sql.c_str(), serverCallback, 0, &zErrMsg);

            if (isDbError) return 0;


            sql = "CREATE TABLE IF NOT EXISTS Discount(code varchar(50),amount int,validity DATE);";
            isDbError = sqlite3_exec(db, sql.c_str(), serverCallback, 0, &zErrMsg);

            if (isDbError) return 0;


            sql = "CREATE TABLE IF NOT EXISTS Attendance(employeeID int,employeeName varchar(256),dateTime DATETIME);";
            isDbError = sqlite3_exec(db, sql.c_str(), serverCallback, 0, &zErrMsg);

            if (isDbError) return 0;

            //sql = "CREATE TABLE IF NOT EXISTS TESTICLE(employeeID int,employeeName varchar(256),dateTime DATETIME);";
            //isDbError = sqlite3_exec(db, sql.c_str(), serverCallback, 0, &zErrMsg);

            if (isDbError) return 0;
            
            return 1;
        }

        void close(){
            sqlite3_close(db);
        }


};