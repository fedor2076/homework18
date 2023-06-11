#pragma once
#include"user.h"

class Message
{
public:

	Message(const string& sender, const string& recipient, const string& letter);
	~Message() = default;

	string getSender() const;
	string getRecipient() const;
	string getLetter() const;

private:

	const string _sender;
	const string _recipient;
	const string _letter;

};
