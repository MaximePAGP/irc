#include "User.hpp"

User::User(): nickName("default"), userName("default"), password(NULL)
	{}


User::User(std::string &nickName, std::string &userName, std::string password)
	: nickName("default"), userName("default"), password(password)
	{}


User::~User() {}


User::User(User const &cpy): userName(cpy.userName), password(password) {}


User	&User::operator=(User const &rhs) {
	if (&rhs == this)
		return *this;
	this->password = rhs.password;
	this->nickName = rhs.nickName;
	return *this;
}