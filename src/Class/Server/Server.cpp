#include "Server.hpp"


Server::Server():portname(NULL), password(NULL) {}

Server::Server(std::string portname, std::string password): 
	portname(portname), password(password) {}

Server::Server(Server const &copy):
	portname(copy.portname),
	password(copy.password),
	serverOps(copy.serverOps),
	canals(copy.canals) {}

Server &Server::operator=(Server const &rhs) {
	if (this != &rhs) {
		this->canals = rhs.canals;
		this->password = rhs.password;
		this->portname = rhs.portname;
		this->serverOps = rhs.serverOps;
	}
	return *this;
}