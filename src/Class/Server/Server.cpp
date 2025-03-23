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

Canal	*Server::getCanalByName(std::string name) const {
	for (std::set<Canal*>::const_iterator it = this->canals.begin(); it != this->canals.end(); it++) {
		std::string tmpName = Canal::replaceSpecialChar((*it)->getName());
		if ((*it)->getName() == name) {
			return *it;
		}
	}
	
	return NULL;	
}


User	*Server::getUserByFd(int fd) const {
	for (std::set<User*>::const_iterator it = this->users.begin(); it != this->users.end(); it++) {
		if ((*it)->getFd().fd == fd) {
			return *it;
		}
	}

	return NULL;
}

User	*Server::getUserByNickname(std::string nickname) const {
	for (std::set<User*>::const_iterator it = this->users.begin(); it != this->users.end(); it++) {
		std::string tmpNick = User::replaceSpecialChar((*it)->getNickName());
		if (tmpNick == nickname) {
			return *it;
		}
	}

	return NULL;
}

User	*Server::getUserByUsername(std::string username) const {
	for (std::set<User*>::const_iterator it = this->users.begin(); it != this->users.end(); it++) {
		std::string tmpUsername = User::replaceSpecialChar((*it)->getNickName());
		if (tmpUsername == username) {
			return *it;
		}
	}

	return NULL;
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
    if (endPosition != std::string::npos) 
	{
        canalName = canalName.substr(0, endPosition);
    }
    if (canalName.empty())
	{
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
    if (canalName[0] != '#') 
	{
        canalName = "#" + canalName;
    }
    // Find or create the channel
    Canal* canal = findCanalByName(canalName);
    if (canal == NULL) 
	{
        // Get the joining user's pollfd
        struct pollfd fd = joiningUser->getFd();
        canal = new Canal(fd, canalName);
        this->addCanal(*canal);   
        std::cout << "Canal " << canalName << " created." << std::endl;
    }
    
    // Add user to the channel
    canal->addUser(*joiningUser);
    
    // Make ALL joining users channel operators
    canal->addChanOps(*joiningUser);
    
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
        if (opIt != channelOps.end()) 
		{
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