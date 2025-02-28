#include "Server.hpp"


Server::Server():  password(NULL), portname(NULL) {}

Server::~Server() {}

Server::Server(std::string portname, std::string password): 
	password(password), portname(portname) {}

Server::Server(Server const &copy):
	password(copy.password),
	serverOps(copy.serverOps),
	canals(copy.canals) {}

Server &Server::operator=(Server const &rhs) {
	if (this != &rhs) {
		this->canals = rhs.canals;
		this->password = rhs.password;
		this->serverOps = rhs.serverOps;
	}
	return *this;
}


std::string Server::getPassord() const {
	return this->password;
}

std::string Server::getPortname() const {
	return this->portname;
}

std::set<User>	Server::getServerOps() const {
	return this->serverOps;
}


std::set<Canal>	Server::getCanals() const {
	return this->canals;
}


std::ostream &operator<<(std::ostream &out, Server const &rhs) {
	out << "Server portname : " << rhs.getPortname() << "\n";
	out << "Server passord : " << rhs.getPassord() << "\n";
	out << "Server Portname : " << rhs.getServerOps() << "\n";
	out << "Server Portname : " << rhs.portname << "\n";
	return out;
}