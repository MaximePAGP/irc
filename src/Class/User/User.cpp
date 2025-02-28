#include "User.hpp"

User::User(): nickName("default"), userName("default"), password(NULL), fd(-1)
	{}


User::User(std::string &nickName, std::string const &userName, std::string password, int fd)
	: nickName(nickName), userName(userName), password(password), fd(fd)
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


std::string	User::getNickName() const {
	return this->nickName;
}

std::string	User::getPassword() const {
	return this->password;
}

std::string	User::getUserName() const {
	return this->userName;
}

void	User::setPassword(std::string newPassord) {
	this->password = newPassord;
}

void	User::setFd(int newfd) {
	this->fd = fd;
}

void	User::setNickName(std::string newNickName) {
	this->nickName = nickName;
}