#pragma once
#include <map>
#include <vector>
#include <memory>
#include "message.h"
#include <fstream>
#include <filesystem>
namespace fs = filesystem;

class LoginException :public exception
{
public:
	LoginException(const std::string& msg) :_msg(msg) {}
	virtual const char* what() const noexcept override
	{
		return "error: user login already exists ";
	}
private:
	std::string _msg;
};

class ConsoleChat
{
public:
	void start();
	bool chatStarted() const;
	void startMenu();
	void signUp();
	void logIn();
	shared_ptr<User> getOnlineUser() const;
	void chatMenu();
	void sendMessage();
	int searchValue(const string& name, const std::map<std::string, std::string>& my_map);
	void startLoad();
private:
	vector<User> _user;
	shared_ptr<User> _onlineUser = nullptr;
	vector<Message> _message;
	bool _chatStarted = false;
	shared_ptr<User> getUserByLogin(const string login) const;
	void openChat() const;
	std::map <string, string>
		main_menu = {
		{"1", "Regisrer"},
		{"2", "Enter chat"},
		{"3", "Leave the chat"},
	};
		std::map <string, string>
		chat_menu = {
		{"1", "Group chat"},
		{"2", "Write a message"},
		{"3", "Exit"},
	};

};
