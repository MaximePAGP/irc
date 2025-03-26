#include "commandManager.hpp"

CommandManager::CommandManager() {}


CommandManager::~CommandManager() {}


/*

	La taille minimum acceptable est 4 car la plus petite commande valide en taille est 4 (KICK ou MODE)

*/
bool CommandManager::hasValidCommand(std::string command) {
	int const validCommandSize = 3;


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
		std::cout << "->" << command << std::endl;
		send(user.getFd().fd, ":server 461 <command> :Not enough parameters\r\n", 46, 0);
		// handle response here
			//should response to client :localhost 421 salut {command} :Unknown command
		return ;
	}
	if (user.getIsConnected() == false || command.find("PASS") == 0) {
		CommandManager::handlePass(command, user);
		return ;
	}

	if (command.find("NICK") == 0) {
		CommandManager::handleNick(command.substr(4, command.size()), user);
		return ;
	} else if (command.find("USER") == 0) {
		CommandManager::handleUsername(command.substr(4, command.size()), user);
		return ;
	}

	// if no handle case return same as 
		//should response to client :localhost 421 salut {command} :Unknown command
}


void	CommandManager::buildCommand(std::string command, int clientFd) {
	Server const &server = Server::getServer();
	User *curUser =server.getUserByFd(clientFd);

	if((curUser->getCommandBuffer().size() + command.size()) > MSG_LEN) {
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


void CommandManager::handleNick(std::string command, User &user) {
	std::string param =  CommandManager::trimParamSpace(command);
	Server const &server = Server::getServer();

	if (param.empty()) {
		//:localhost 431 ${nickname} ${newNickname} :No nickname given
		return;
	}

	if (param.size() > LIMIT_USERNAME_NICKNAME) {
		// need response
		return;
	}

	if (server.getUserByNickname(param) != NULL) {
		// nickname already takken;
		return ;
	}

	if (hasForbiddenNickChar(param)) {
		// :localhost 432 ${nickname} ${nickname} :Nickname is unavailable: Illegal characters
		return;
	}

	user.setNickName(param);
	std::cout << "New nickname: <" << user.getNickName() << ">" << std::endl;
	// send succes response
}


void CommandManager::handleUsername(std::string command, User &user) {
	std::string param =  CommandManager::trimParamSpace(command);
	Server const &server = Server::getServer();

	if (param.empty()) {
		//:localhost 431 ${nickname} ${newNickname} :No nickname given
		return;
	}

	if (param.size() > LIMIT_USERNAME_NICKNAME) {
		// need response
		return;
	}

	if (server.getUserByUsername(param) != NULL) {
		// nickname already takken;
		return ;
	}

	if (hasForbiddenUsernameChar(param)) {
		// :localhost 432 ${nickname} ${nickname} :Nickname is unavailable: Illegal characters
		return;
	}

	if (user.getUserName().size() != 0) {
		// :localhost 462 ${username} :You may not reregisterw
		return;
	}

	user.setUsername(param);
	std::cout << "New UserName: <" << user.getUserName() << ">" << std::endl;
	// send succes response
}


std::string CommandManager::trimParamSpace(std::string param) {
	size_t start = param.find_first_not_of(" \t\r\n");

	if (start == std::string::npos)
		return "";

	size_t end = param.find_first_of(" \t\r\n", start);
	std::string trimParam = "";

	if (end != std::string::npos)
		trimParam = param.substr(start, end - start);
	else
		trimParam = param.substr(start);

	size_t lastNonSpace = trimParam.find_last_not_of(" \t\r\n");

	if (lastNonSpace != std::string::npos) {
		trimParam = trimParam.substr(0, lastNonSpace + 1);
	}

	return trimParam;
}

bool	CommandManager::hasForbiddenNickChar(std::string nickname) {
	if (nickname.empty())
		return true;
	
	if (nickname.find_first_of("-") == 0)
		return true;

	if (nickname.find_first_of("/<>.,:;'\"()?¿!~@#%^$&*+=") != std::string::npos)
		return true;

	for (size_t i = 0; i < nickname.size(); i++)
	{
		if (!::isascii(nickname[i]))
			return true;
	}

	return false;
}

bool	CommandManager::hasForbiddenUsernameChar(std::string usnername) {
	if (usnername.empty())
		return true;
	
	if (usnername.find_first_of("-") == 0)
		return true;

	if (usnername.find_first_of("/<>.,:;'\"()?¿!~@#%^$&*+=") != std::string::npos)
		return true;

	for (size_t i = 0; i < usnername.size(); i++)
	{
		if (!::isascii(usnername[i]))
			return true;
	}

	return false;
}

