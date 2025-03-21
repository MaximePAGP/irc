#include "Server.hpp"
#include "string.h"

static	Server *server = NULL;

Server::Server(): password(NULL), portname(0), isRunning(false) {}

Server::~Server() {

	for (size_t i = 0; i < this->sockets.size(); i++)
	{
		close(this->sockets[i].fd);
	}
	this->sockets.clear();
	
	for (std::set<User *>::iterator it = this->users.begin(); it != this->users.end(); it++) {
		delete *it; 
	}
	this->users.clear();

	for (std::set<Canal *>::iterator it = this->canals.begin(); it != this->canals.end(); it++) {
		delete *it; 
	}
	this->canals.clear();

	this->serverOps.clear();
}

Server::Server(int portname, std::string password): 
	password(password), portname(portname) {
		if (portname < 1 || portname > MAX_PORT)
			throw PortOutOfRangeException();
}

Server &Server::init(int portname, std::string password) {
    if (!server)
        server = new Server(portname, password);
    return *server;
}

void	Server::kill() {
	if (!server)
		return ;
	this->setState(false);
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

std::set<User*>	Server::getServerOps() const {
	return this->serverOps;
}

std::set<User*>	Server::getUsers() const {
	return this->users;
}

std::set<Canal*>	Server::getCanals() const {
	return this->canals;
}

std::pair<std::set<Canal*>::iterator, bool>	Server::addCanal(Canal &newCanal) {
	return this->canals.insert(&newCanal);
}

std::size_t	Server::removeCanal(Canal &target) {
	return this->canals.erase(&target);
}

std::pair<std::set<User*>::iterator, bool>	Server::addUser(User &newUser) {
	return this->users.insert(&newUser);
}

std::size_t	Server::removeUser(User &target) {
	return this->users.erase(&target);
}

std::pair<std::set<User*>::iterator, bool>	Server::addServerOps(User &newServerOp) {
	return this->serverOps.insert(&newServerOp);
}

std::size_t	Server::removeServerOps(User &target) {
	return this->serverOps.erase(&target);
}

void	Server::running() {
	this->initServerSocket();
	this->bindAndListenPort();
	this->setState(true);
	std::cout << "Server is running on port : " << this->getPortname() << std::endl;
	while (this->getState()) {
		int	pollEvent = poll(this->sockets.data(), this->sockets.size(), -1);
		if (pollEvent == -1)
			break;
		// std::cout << "pollevent : " << pollEvent << std::endl;
		// if (pollEvent < 1)
		// 	continue;

		for (size_t i = 0; i < this->sockets.size(); i++) {
			if (this->sockets[i].revents & POLLIN) {
				std::cout << "trigger " << std::endl;
				if (i == 0)
					this->createNewClient();
				else
					this->handleClientMsg(this->sockets[i].fd);
			}
		}
	}
}


void	Server::bindAndListenPort() {
	struct sockaddr_in	address;
	int				option = 1;	

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(this->getPortname());
	if (setsockopt(this->sockets[0].fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) == -1)
        throw ServerExepction::CannotSetSocketOptionException();
	if (bind(this->sockets[0].fd, (struct sockaddr*) &address, sizeof(address)) == -1)
		throw ServerExepction::CannotBindPortException();
	if (listen(this->sockets[0].fd, 10) < 0)
		throw ServerExepction::CannotListenException();
	if (fcntl(this->sockets[0].fd, F_SETFL, O_NONBLOCK) == -1)
		throw ServerExepction::CannotSetFdOptionsException();
}

void	Server::initServerSocket() {
	struct pollfd serverPollFd;

	serverPollFd.fd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverPollFd.fd < 0)
		throw SocketCreationException();
	serverPollFd.events = POLLIN;
	this->sockets.push_back(serverPollFd);
}

void	Server::handleClientMsg(int clientFd) {
	char buffer[MSG_LEN];
	ssize_t bytesRead = 1;

	while ((bytesRead = recv(clientFd, buffer, sizeof(buffer), 0)) > 0) {
		std::string msg(buffer);
		std::cout << "Received : " << msg << std::endl;
		if (msg.find("JOIN") == 0) {
			handleJoinCanal(clientFd, msg);
		}
		send(clientFd, "Message received", 16, 0);
	}
	// handle if recv crash send a response to client to say we cannot handle the commmand
	if (bytesRead == 0) {
		this->handleClientLogout(clientFd);
	}

}

void	Server::handleClientLogout(int clientFd) 
{

	for (std::vector<struct pollfd>::iterator it = this->sockets.begin(); it != this->sockets.end(); it++)
	{
		pollfd cur = *it;
		if (cur.fd == clientFd) {
			this->sockets.erase(it);
			break ;
		}
	}
	

	for (std::set<User*>::iterator it = this->users.begin(); it != this->users.end();)
	{
		User *cur = *it;
		if (cur != NULL && cur->getFd().fd == clientFd) {
			this->users.erase(it);  
	 		delete cur;
			break;
		} else {
			it++;
		}
	}

	close(clientFd);
}

void Server::createNewClient() {
    struct sockaddr_in clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    int clientFd = accept(this->sockets[0].fd, (struct sockaddr*)&clientAddr, &addrLen);
    if (clientFd < 0) {
		std::cerr << "New connexion was failed" << std::endl;
		return ;
	}
	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1) {
		std::cerr << "Fcntl crash and close connection !" << std::endl;
		close(clientFd);
		return ;
	}
	std::cout << "New client connected : " << clientFd << std::endl;
	struct pollfd newClient;

	newClient.fd = clientFd;
	newClient.events = POLLIN;
	newClient.revents = 0;
	this->sockets.push_back(newClient);
	User *user = new User("", "", "");
	user->setFd(newClient);
	this->addUser(*user);
}

bool Server::hasUser(std::set<User> usersContainer, User &target) {
	return usersContainer.find(target) != usersContainer.end();
}

std::ostream &operator<<(std::ostream &out, Server const &rhs) {
	out << "Server portname : " << rhs.getPortname() << "\n";
	out << "Server passord : " << rhs.getPassord() << "\n";
	out << "Server Server ops : " << rhs.getServerOps().size() << "\n";
	printUsers(rhs.getServerOps());
	out << "Server canals : " << rhs.getServerOps().size() << "\n";
	printCanals(rhs.getCanals());
	return out;
}

Canal* Server::findCanalByName(const std::string& name) {
    for (std::set<Canal*>::iterator it = this->canals.begin(); it != this->canals.end(); ++it) 
	{
		Canal* canal = *it;
        if (canal->getName() == name) 
		{
            return canal;
        }
    }
    return NULL;
}

User* Server::findUserByFd(int clientFd)
{
    std::set<User*>::iterator it;
    for (it = this->users.begin(); it != this->users.end(); ++it) 
    {
        User* user = *it;
        if (user->getFd().fd == clientFd) {
            return user;
        }
    }
    return NULL;
}

void Server::handleJoinCanal(int clientFd, const std::string msg)
{
    // Find space after "JOIN"
    size_t pos = msg.find(' ');
    if (pos == std::string::npos) 
    {
        std::cerr << "Invalid JOIN command format" << std::endl;
        return;
    }
    
    // Extract channel name
    std::string canalName = msg.substr(pos + 1);
    
    // Remove any trailing whitespace or newlines
    size_t endPosition = canalName.find_first_of(" \r\n");
    if (endPosition != std::string::npos) {
        canalName = canalName.substr(0, endPosition);
    }
    
    if (canalName.empty()) {
        std::cerr << "Channel name is empty" << std::endl;
        return;
    }
    
    // Find the user who's joining
    User* joiningUser = findUserByFd(clientFd);
    if (joiningUser == NULL) {
        std::cerr << "User not found for clientFd: " << clientFd << std::endl;
        return;
    }
    
    // Validate channel name
    if (canalName[0] != '#') {
        canalName = "#" + canalName;
    }
    
    // Find or create the channel
    Canal* canal = findCanalByName(canalName);
    
    if (canal == NULL) {
        // Get the joining user's pollfd
        struct pollfd fd = joiningUser->getFd();
        canal = new Canal(fd, canalName);
        this->addCanal(*canal);
        
        // Make the joining user a channel operator
        canal->addChanOps(*joiningUser);
        
        std::cout << "Canal " << canalName << " created." << std::endl;
    }
    
    // Add user to the channel
    canal->addUser(*joiningUser);
    
    // Send JOIN confirmation to the user
    std::string joinResponse = ":" + joiningUser->getNickName() + " JOIN " + canalName + "\r\n";
    send(clientFd, joinResponse.c_str(), joinResponse.length(), 0);
    
    // Send channel topic if it exists
    std::string topic = canal->getTopic();
    if (!topic.empty()) {
        std::string topicResponse = ":server 332 " + joiningUser->getNickName() + " " + canalName + " :" + topic + "\r\n";
        send(clientFd, topicResponse.c_str(), topicResponse.length(), 0);
    }
    
    // Send names list (users in channel)
    std::string namesResponse = ":server 353 " + joiningUser->getNickName() + " = " + canalName + " :";
    
    std::set<User*> channelUsers = canal->getCurrentUsers();
    std::set<User*> channelOps = canal->getChanOps();
    std::set<User*>::iterator userIt;
    
    for (userIt = channelUsers.begin(); userIt != channelUsers.end(); ++userIt) {
        User* channelUser = *userIt;
        
        // Check if this user is a channel operator
        std::set<User*>::iterator opIt = channelOps.find(channelUser);
        if (opIt != channelOps.end()) {
            namesResponse += "@";
        }
        
        namesResponse += channelUser->getNickName() + " ";
    }
    
    namesResponse += "\r\n";
    send(clientFd, namesResponse.c_str(), namesResponse.length(), 0);
    
    // End of names list
    std::string endNamesResponse = ":server 366 " + joiningUser->getNickName() + " " + canalName + " :End of /NAMES list.\r\n";
    send(clientFd, endNamesResponse.c_str(), endNamesResponse.length(), 0);
    
    // Notify other users in the channel
    for (userIt = channelUsers.begin(); userIt != channelUsers.end(); ++userIt) {
        User* channelUser = *userIt;
        
        // Skip the joining user (already notified)
        if (channelUser == joiningUser) {
            continue;
        }
        
        int userFd = channelUser->getFd().fd;
        send(userFd, joinResponse.c_str(), joinResponse.length(), 0);
    }
    
    std::cout << "User " << joiningUser->getNickName() << " joined canal " << canalName << std::endl;
}