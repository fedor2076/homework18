#include "consolechat.h"
#include <iostream>

bool ConsoleChat::chatStarted() const
{
	return _chatStarted;
}

void ConsoleChat::start()
{
	_chatStarted = true;
}
void ConsoleChat::startLoad()
{
	string file_n = "users";

	std::string login, password, name;
	std::ifstream in(file_n);

	if(!in.is_open())
	{
	std::ofstream in(file_n);
	fs::permissions(file_n ,
			fs::perms::group_write | fs::perms::group_read | fs::perms::others_all,
			fs::perm_options::remove);
			in.close();
	}

	if(in.is_open())
	{
		while(std::getline(in, login) && std::getline(in, password) && std::getline(in, name))
		{
		User user = User(login, password, name);
		_user.push_back(user);
    }
		fs::permissions(file_n ,
				fs::perms::group_write | fs::perms::group_read | fs::perms::others_all,
				fs::perm_options::remove);
	}
 	in.close();

//---------------------------------------------------------------------

	file_n = "messages";

	std::string from, to, text;
	in.open(file_n);

	if(!in.is_open())
	{
	std::ofstream in(file_n);
	fs::permissions(file_n ,
			fs::perms::group_write | fs::perms::group_read | fs::perms::others_all,
			fs::perm_options::remove);
			in.close();
	}
	if(in.is_open())
	{
		while(std::getline(in, from) && std::getline(in, to) && std::getline(in, text))
		{
			Message message = Message(from, to, text);
			_message.push_back(message);
    }
		fs::permissions(file_n ,
				fs::perms::group_write | fs::perms::group_read | fs::perms::others_all,
				fs::perm_options::remove);
	}
 	in.close();

//-------------------------------------------------------------------

	start();
}


int ConsoleChat::searchValue(const string& name, const std::map<std::string, std::string>& my_map)
{
	int i = 1;
	for (auto& element : my_map)
	{
		if (name == element.first)
					return i;
				i++;
	}
	return 0;
}
void ConsoleChat::startMenu()
{
	std::cout << "\tWelcome to chat!\n\n" << "Choose an action to continue: \n\n";

	for (auto& element : main_menu)
	{
		std::cout << element.first << " " << element.second << std::endl;
	}

	string action;


	std::cin >> action;

	switch (searchValue(action, main_menu))
	{
	case 1:
		signUp();
		break;
	case 2:

		logIn();
		break;
	case 3:
		_chatStarted = false;
		break;
	default:
		std::cout << "Please check that the characters you are entering are correct.\n";
		break;
	}

};

shared_ptr<User> ConsoleChat::getUserByLogin(const string login) const
{
	for (auto& user : _user)
	{
		if (login == user.getLogin())
			return make_shared<User>(user);
	}
	return nullptr;
}

shared_ptr<User>  ConsoleChat::getOnlineUser() const
{
	return _onlineUser;
}

void ConsoleChat::logIn()
{
	string login;
	string password;

	std::cout << "Please enter a login: ";
	std::cin >> login;
	std::cout << "Enter password: ";
	std::cin >> password;

	_onlineUser = getUserByLogin(login);

	if (_onlineUser == nullptr || password != _onlineUser->getPassword())
	{
		_onlineUser = nullptr;
		std::cout << "Invalid login or password, please try again\n";
	}
}

void ConsoleChat::signUp()
{
	string login, password, name;

	std::cout << "Please enter your login: ";
	std::cin >> login;
	std::cout << "Choose security password: ";
	std::cin >> password;
	std::cout << "Enter your name: ";
	std::cin >> name;

	try
	{
		if (getUserByLogin(login) || login == "all")
		{
			throw LoginException("error: ");
		};
	}

	catch (const LoginException& ex)
	{
		cerr << "\n\n (" << ex.what() << ") \n\n";
		return;
	}

	User user = User(login, password, name);
	_user.push_back(user);
	_onlineUser = make_shared<User>(user);


	std::ofstream out("users", std::ios::app);
	if(out.is_open())
	{
			out<<user.getLogin()<<"\n"<<user.getPassword()<<"\n"<<user.getName()<<"\n";
	}
	out.close();
	std::cout << "\nCongratulations! You have successfully registered!\n";
}

void ConsoleChat::openChat() const
{
	string sender, recipient;

	std::cout << "-------users chatting online-----\n";
	std::cout << " login:  \n";

	for (auto& user : _user)
		std::cout << " " << user.getLogin() << "\n";

	std::cout << "\n--------"<<"login: "<<_onlineUser->getLogin()<<" Name: "<<_onlineUser->getName()<<"-----------------\n\n";

	for (auto& message : _message)
	{
		if (_onlineUser->getLogin() == message.getSender() || _onlineUser->getLogin() == message.getRecipient() || message.getRecipient() == "all")
		{
			sender = (_onlineUser->getLogin() == message.getSender()) ? _onlineUser->getLogin()  : getUserByLogin(message.getSender())->getLogin();

			if (message.getRecipient() == "all")
			{
				recipient = "all";
			}
			else
			{
				recipient = (_onlineUser->getLogin() == message.getRecipient()) ? _onlineUser->getLogin() : getUserByLogin(message.getRecipient())->getLogin();
			}
			std::cout << "message from " << sender << ": to " << recipient << "\n";
			std::cout << " - " << message.getLetter() << " - \n";
		}
	}
	std::cout << "---------------------------------\n\n";

}

void ConsoleChat::sendMessage()
{
	string recipient, text;
	std::ofstream out("messages", std::ios::app);
	std::cout << "to (login or all) - ";
	std::cin >> recipient;
	std::cout << ">> ";
	std::cin.ignore();
	getline(std::cin, text);

	if (!(recipient == "all" || getUserByLogin(recipient)))
	{
		std::cout << "error no such user  -- " << recipient << "\n";
		return;
	}
	if (recipient == "all")
	{
		_message.push_back(Message(_onlineUser->getLogin(), "all", text));
		if(out.is_open())
		{
			out<<_onlineUser->getLogin()<<"\n"<<"all"<<"\n"<<text<<"\n";
		}
	}
	else
	{
		_message.push_back(Message(_onlineUser->getLogin(), getUserByLogin(recipient)->getLogin(), text));
		std::cout << "message send -  " << recipient << ":\n ";
		out<<_onlineUser->getLogin()<<"\n"<<getUserByLogin(recipient)->getLogin()<<"\n"<<text<<"\n";
	}

	out.close();

}

void ConsoleChat::chatMenu()
{
	std::cout << "Welcome " << _onlineUser->getName() << "\n";
	while (_onlineUser)
	{
		std::cout << "Choose an action: \n";
		for (auto& element : chat_menu)
		{
			std::cout << element.first << " " << element.second << std::endl;
		}
		string action;
		std::cin >> action;

		switch (searchValue(action, chat_menu))
		{
		case 1:
			openChat();
			break;
		case 2:
			sendMessage();
			break;
		case 3:
			_onlineUser = nullptr;
			break;
		default:
			std::cout << "Please try again\n";
			break;
		}
	}
}
