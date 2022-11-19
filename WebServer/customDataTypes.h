#pragma once
#define MAX 1000

typedef struct LoginStuff {
	std::string employeeID;
	std::string name;
	std::string role;
	std::string post;
	std::string username;
	std::string password;
	bool didLogin;
} LoginDetails;