/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magrondi <magrondi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 04:50:56 by leye              #+#    #+#             */
/*   Updated: 2025/05/27 09:51:46 by magrondi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

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

	for (std::set<Channel *>::iterator it = this->channels.begin(); it != this->channels.end(); it++) {
		delete *it; 
	}
	this->channels.clear();

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

std::string Server::getPassword() const {
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

std::set<Channel*>	Server::getChannels() const {
	return this->channels;
}

std::pair<std::set<Channel*>::iterator, bool>	Server::addCanal(Channel &newCanal) {
	return this->channels.insert(&newCanal);
}

std::size_t	Server::removeCanal(Channel &target) {
	return this->channels.erase(&target);
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

Channel	*Server::getChannelByName(std::string name) const {
	for (std::set<Channel*>::const_iterator it = this->channels.begin(); it != this->channels.end(); it++) {
		std::string tmpName = Channel::replaceSpecialChar((*it)->getName());
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
		std::string remplacedNick = User::replaceSpecialChar(nickname);
		if (tmpNick == remplacedNick)
			return *it;
	}

	return NULL;
}

User	*Server::getUserByUsername(std::string username) const {
	for (std::set<User*>::const_iterator it = this->users.begin(); it != this->users.end(); it++) {
		std::string tmpUsername = User::replaceSpecialChar((*it)->getUserName());
		std::string changedUsername = User::replaceSpecialChar(username);
		if (tmpUsername == changedUsername)
			return *it;
	}

	return NULL;
}


bool Server::hasUser(std::set<User> usersContainer, User &target) 
{
	return usersContainer.find(target) != usersContainer.end();
}

std::ostream &operator<<(std::ostream &out, Server const &rhs) {
	out << "Server portname : " << rhs.getPortname() << "\n";
	out << "Server passord : " << rhs.getPassword() << "\n";
	out << "Server Server ops : " << rhs.getServerOps().size() << "\n";
	printUsers(rhs.getServerOps());
	out << "Server canals : " << rhs.getServerOps().size() << "\n";
	printChannels(rhs.getChannels());
	return out;
}

Channel* Server::findChannelByName(const std::string& name) {
	for (std::set<Channel*>::iterator it = this->channels.begin(); it != this->channels.end(); ++it)
	{
		Channel* canal = *it;
		if (canal->getName() == name) 
			return canal;
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


void	Server::psend(int fd, const void *buffer, size_t size, int flags) {
		if (send(fd, buffer, size, flags) < 0)
			throw SendCrashException();
}


void	Server::pclose(int fd) {
	if (close(fd) < 0)
		throw CloseCrashException();
}
