#include "UserManager.hpp"



bool	UserManager::isUniqueNickName(std::string const &nickName) {
	Server const server = Server::getServer();

	std::set<User *> const connetedUsers = server.getUsers();

	for (std::set<User *>::iterator it = connetedUsers.begin(); it != connetedUsers.end(); it++) {
		User *curUser = *it;
		if (!curUser->getNickName().empty() && curUser->getNickName().compare(nickName))
			return false;
	}

	return true;
}

bool	UserManager::isUniqueUsername(std::string const &username) {
	Server const server = Server::getServer();

	std::set<User *> const connetedUsers = server.getUsers();

	for (std::set<User *>::iterator it = connetedUsers.begin(); it != connetedUsers.end(); it++) {
		User *curUser = *it;
		if (!curUser->getUserName().empty() && curUser->getUserName().compare(username))
			return false;
	}

	return true;
}


// void    UserManager::createNewUser(std::string clientMsg, int fd) {
	
// }