#include "User.hpp"

User::User(): nickName("default"), userName("default"), password(NULL)
	{}


User::User(std::string const &nickName, std::string const &userName, std::string password)
	: nickName(nickName), userName(userName), password(password)
	{}


User::~User() {}


User::User(User const &cpy): nickName(cpy.nickName), password(cpy.password) {}


User	&User::operator=(User const &rhs) {
	if (this != &rhs) {
		this->password = rhs.password;
		this->nickName = rhs.nickName;
		this->fd.events = rhs.fd.events;
		this->fd.fd = rhs.fd.fd;
		this->fd.revents = rhs.fd.revents;
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

struct	pollfd	User::getFd() const {
	return this->fd;
}

void	User::setPassword(std::string newPassord) {
	this->password = newPassord;
}

void	User::setFd(struct pollfd newfd) {
	this->fd = newfd;
}

void	User::setNickName(std::string newNickName) {
	this->nickName = newNickName;
}

bool	User::operator<(const User &other) const {
	return this->getNickName() < other.getNickName();
}

std::ostream &operator<<(std::ostream &out, User const &rhs) {
	out << " " << rhs.getNickName() << " " << rhs.getUserName() << " " << rhs.getPassword() << "\n";
	return out;
}

void	printUsers(std::set<User*> users) {
	for (std::set<User*>::iterator it = users.begin(); it != users.end(); ++it)
    	std::cout << *it;
}