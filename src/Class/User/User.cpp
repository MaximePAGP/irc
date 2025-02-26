#include "User.hpp"

User::User(): nickName("default"), userName("default"), password(NULL)
	{}


User::User(std::string &nickName, std::string const &userName, std::string password)
	: nickName(nickName), userName(userName), password(password)
	{}


User::~User() {}


User::User(User const &cpy): nickName(cpy.nickName), password(cpy.password) {}


User	&User::operator=(User const &rhs) {
	if (this != &rhs) {
		this->password = rhs.password;
		this->nickName = rhs.nickName;
	}
	return *this;
}
