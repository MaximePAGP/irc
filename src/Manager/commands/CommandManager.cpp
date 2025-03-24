#include "CommandManager.hpp"

CommandManager::CommandManager() {}


CommandManager::~CommandManager() {}


/*

	La taille minimum acceptable est 4 car la plus petite commande valide en taille est 4 (KICK ou MODE)

*/
bool CommandManager::hasValidCommand(std::string command) {
	int const validCommandSize = 4;

	size_t firstCharIndex = command.find_first_not_of(" \t\r\n");
	if (firstCharIndex == std::string::npos)
		return false;

	size_t firstSpaceIndex = command.find_first_of(" \t\r\n", firstCharIndex);
	if (firstSpaceIndex == std::string::npos)
		return false;

	if ((firstSpaceIndex - firstCharIndex) < validCommandSize)
		return false;

	for (size_t i = firstCharIndex; i < firstSpaceIndex; i++) {
		if (!std::isalpha(command[i]))
			return false;
	}

	return true;
}


void	CommandManager::redirectCommand(std::string command, User &user) {
	(void)user;
	if (command.empty())
		return ;

	if (!CommandManager::hasValidCommand(command)) {
		// handle response here
			//should response to client :localhost 421 salut {command} :Unknown command
		return ;
	}

	if (command.find("NICK") == 0) {
		CommandManager::handleNick(command.substr(4, command.size()), user);
		return ;
	} else if (command.find("USER") == 0) {
		CommandManager::handleUsername(command.substr(4, command.size()), user);
		return ;
	} else if (command.find("MODE") == 0) {
		CommandManager::handleMode(command.substr(4, command.size()), user);
		return ;
	}// Dans CommandManager::redirectCommand, ajoutez:
	else if (command.find("JOIN") == 0) {
		CommandManager::handleJoin(command.substr(4, command.size()), user);
		return;
	}
	// if no handle case return same as 
		//should response to client :localhost 421 salut {command} :Unknown command
}


void	CommandManager::buildCommand(std::string command, int clientFd) {
	Server const &server = Server::getServer();
	User *curUser = server.getUserByFd(clientFd);
	
	if ((curUser->getCommandBuffer().size() + command.size()) > MSG_LEN) {
		// maybe send error to client ?
		curUser->flushCommandBuffer();
		return;
	}
	curUser->setCommandBuffer(
		curUser->getCommandBuffer().append(command)
	);

	while (curUser->getCommandBuffer().find("\r\n") != std::string::npos) {
		size_t pos = curUser->getCommandBuffer().find("\r\n");
	
		std::string fullCommand = curUser->getCommandBuffer().substr(0, pos);
		curUser->setCommandBuffer(curUser->getCommandBuffer().substr(pos + 2)); // 2 is the length of \r\n
		
		CommandManager::redirectCommand(fullCommand, *curUser);
	}
}


std::string CommandManager::trimFirstParamSpace(std::string param) {
	size_t start = param.find_first_not_of(" \r\n");

	if (start == std::string::npos)
		return "";

	size_t end = param.find_first_of(" \r\n", start);
	std::string trimParam = "";

	if (end != std::string::npos)
		trimParam = param.substr(start, end - start);
	else
		trimParam = param.substr(start);

	size_t lastNonSpace = trimParam.find_last_not_of(" \r\n");

	if (lastNonSpace != std::string::npos) {
		trimParam = trimParam.substr(0, lastNonSpace + 1);
	}

	return trimParam;
}

