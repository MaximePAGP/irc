#include "Server.hpp"

static	Server *server = NULL;

Server::Server(): password(NULL), portname(0), socketFd(-1), isRunning(false) {}

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
		this->isRunning = rhs.isRunning;
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

bool Server::getState() const {
	return this->isRunning;
}

void Server::setState(bool value) {
	this->isRunning = value;
}

void Server::setPassword(std::string value) {
	this->password = value;
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
	this->setState(true);
	std::cout << "Server is running on port : " << this->getPortname() << std::endl;
	while (this->getState()) {
		int clientFd;
		struct sockaddr_in clientAddr;

		socklen_t clientAddrSize = sizeof(clientAddr);
		clientFd = accept(this->socketFd, (struct sockaddr*)&clientAddr, &clientAddrSize);
		if (clientFd == -1) {
			std::cerr << "Error: Failed to accept connection" << std::endl;
			continue;
		}
		std::cout << "New client connected!" << std::endl;
		handleClient(clientFd);
	}
}


void	Server::bindAndListenPort() {
	struct sockaddr_in address;

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(this->getPortname());
	if (bind(this->socketFd, (struct sockaddr*) &address, sizeof(address)) == -1)
		throw ServerExepction::CannotBindPortException();
	if (listen(this->socketFd, 10) < 0)
		throw ServerExepction::CannotListenException();
}

void	Server::createSocket() {
	this->socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->socketFd < 0)
		throw SocketCreationException();
}

void	Server::handleClient(int clientFd) {
	char buffer[MSG_LEN];
	ssize_t bytesRead;

	while ((bytesRead = recv(clientFd, buffer, sizeof(buffer), 0)) > 0) {
		buffer[bytesRead] = '\0';
		std::cout << "Received: " << buffer << std::endl;
		send(clientFd, "Message received", 16, 0);
	}
	close(clientFd);
	std::cout << "Client disconnected" << std::endl;
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