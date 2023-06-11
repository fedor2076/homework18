#pragma once
#include<string>

using namespace std;

class User
{
public:

	User(const string& login, string& password, string& name);
	~User() = default;

	string getLogin() const; 
	string getPassword() const; 
	string getName() const; 

private:

	string _login;
	string _password;
	string _name;

};