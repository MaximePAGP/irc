#include "Server.hpp"

static	Server *server = NULL;

Server::Server(): password(NULL), portname(0), socketFd(-1) {}

Server::~Server() {}

Server::Server(int portname, std::string password): 
	password(password), portname(portname) {}

Server::Server(Server const &copy):
	password(copy.password),
	portname(copy.portname),
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
        server = new Server(portname, password);
    return *server;
}

void	Server::kill() {
	if (!server)
		return ;
	if (this->socketFd > 0)
		close(this->socketFd);
	delete server;
}

Server &Server::getServer() {
    return *server;
}

std::string Server::getPassord() const {
	return this->password;
}

int Server::getPortname() const {
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

void	Server::running() {
	this->createSocket();
	this->bindAndListenPort();
}


void	Server::bindAndListenPort() {
	struct sockaddr_in address;

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(this->getPortname());

	if (bind(this->socketFd, (struct sockaddr*) &address, sizeof(address)) == -1)
		throw ServerExepction::CannotBindPortException();
    if (listen(this->socketFd, 10) == -1)
		throw ServerExepction::CannotListenException();
}

void	Server::createSocket() {
	this->socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->socketFd == -1)
		throw SocketCreationException();
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