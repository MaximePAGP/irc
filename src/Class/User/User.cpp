#include "User.hpp"

User::User(): nickName("default"), userName("default"), password(NULL)
	{
		this->fd.fd = -1;
		this->isConnected = false;
	}


User::User(std::string nickName, std::string userName, std::string password)
	: nickName(nickName), userName(userName), password(password)
	{
		this->fd.fd = -1;
		this->isConnected = false;
	}


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

std::string	User::getCommandBuffer() const {
	return this->commandBuffer;
}

bool	User::getIsConnected() const {
	return this->isConnected;
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

void	User::setUsername(std::string newUserName) {
	this->userName = newUserName;
}

void	User::setCommandBuffer(std::string value) {
	this->commandBuffer = value;
}

void	User::setIsConnected(bool status) {
	this->isConnected = status;
}

bool	User::operator<(const User &other) const {
	return this->getNickName() < other.getNickName();
}

std::ostream &operator<<(std::ostream &out, User const &rhs) {
	out << " " << rhs.getNickName() << " " << rhs.getUserName() << " " << rhs.getPassword() << " " << rhs.getCommandBuffer() << "\n";
	return out;
}

void	User::flushCommandBuffer() {
	this->setCommandBuffer("");
}

std::string User::replaceSpecialChar(std::string param) {
	std::string tmp = param;

	for (size_t i = 0; i < tmp.size(); i++)
	{
		if (tmp[i] == '{' || tmp[i] == '[')
			tmp[i] = ',';
		else if (tmp[i] == '}' || tmp[i] == ']')
			tmp[i] = ':';
	}
	
	return tmp;
}

void	printUsers(std::set<User*> users) {
	for (std::set<User*>::iterator it = users.begin(); it != users.end(); ++it)
    	std::cout << *it;
}
