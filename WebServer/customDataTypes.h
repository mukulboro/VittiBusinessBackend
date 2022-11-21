#pragma once
#define MAX 1000
#include <ctime>

std::string getDateTime() {
	
	std::string toReturn, y, m, d, hr, min, sec;;
	time_t now = time(0);
	tm* ltm = localtime(&now);

	y = std::to_string(ltm->tm_year - 100 + 2000);
	m = std::to_string(ltm->tm_mon + 1);
	d = std::to_string(ltm->tm_mday);
	hr = std::to_string(ltm->tm_hour);
	min = std::to_string(ltm->tm_min);
	sec = std::to_string(ltm->tm_sec);

	toReturn = y+"/"+m+"/"+d + " " + hr+":"+min+":"+sec;

	return toReturn;
}

typedef struct LoginStuff {
	std::string employeeID;
	std::string name;
	std::string role;
	std::string post;
	std::string username;
	std::string password;
	bool didLogin;
} LoginDetails;

typedef struct DiscountStuff {
	std::string code;
	std::string amount; //denotes percentage
	std::string validity;
} DiscountDetails;

typedef struct AttendanceStuff {
	std::string employeeID;
	std::string employeeName;
	std::string datetime;
} AttendanceDetails;

typedef struct ProductStuff {
	std::string productID;
	std::string productName;
	std::string minPrice;
	std::string maxPrice;
	std::string price;
	std::string stock;
} ProductDetails;

typedef struct OrderStuff {
	std::string orderID;
	std::string productID;
	std::string customerName;
	std::string customerContact;
	std::string customerAddress;
	std::string price;
} OrderDetails;

typedef struct AgendaStuff {
	std::string agenda;
	std::string postedDate;
} AgendaDetails;