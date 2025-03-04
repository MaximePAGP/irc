#include "Server.hpp"

static	Server *server = NULL;

Server::Server():  password(NULL), portname(0) {}

Server::~Server() {}

Server::Server(int portname, std::string password): 
	password(password), portname(portname) {}

Server::Server(Server const &copy):
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

Server &Server::init(int portname, std::string password) {
    if (!server)
	{
        server = new Server(portname, password);	
	}
    return *server;
}

Server &Server::getServer() {
    return *server;
}

std::string Server::getPassord() const {
	return this->password;
}

int	Server::getPortname() const {
	return this->portname;
}

std::set<User>	Server::getServerOps() const {
	return this->serverOps;
}

std::set<Canal>	Server::getCanals() const {
	return this->canals;
}

std::pair<std::set<Canal>::iterator, bool>	Server::addCanal(Canal newCanal) {
	return this->canals.insert(newCanal);
}

std::size_t	Server::removeCanal(Canal target) {
	return this->canals.erase(target);
}

std::pair<std::set<User>::iterator, bool>	Server::addServerOps(User newServerOp) {
	return this->serverOps.insert(newServerOp);
}

std::size_t	Server::removeServerOps(User target) {
	return this->serverOps.erase(target);
}


std::ostream &operator<<(std::ostream &out, Server const &rhs) {
	out << "Server portname : " << rhs.getPortname() << "\n";
	out << "Server passord : " << rhs.getPassord() << "\n";
	out << "Server Server ops : " << rhs.getServerOps().size() << "\n";
	printUsers(rhs.getServerOps());
	out << "Server canals : " << rhs.getServerOps().size() << "\n";
	printCanal(rhs.getCanals());
	return out;
}