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