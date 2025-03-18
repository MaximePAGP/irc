#include "commandManager.hpp"

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


bool	CommandManager::isNick(std::string command) {
	if (command.find("nick") == 0)
		return true;
	return false;
}

void	CommandManager::redirectCommand(std::string command, User const &user) {
	(void)user;
	if (command.empty())
		return ;
	std::string cmdCpy = command;
	std::transform(cmdCpy.begin(), cmdCpy.end(), cmdCpy.begin(), ::tolower);

	// std::cout << "je suis là " << std::endl;

	if (!CommandManager::hasValidCommand(cmdCpy)) {
		// handle response here
			//should response to client :localhost 421 salut {command} :Unknown command
		return ;
	}
	std::cout << "je suis là " << std::endl;

	if (CommandManager::isNick(cmdCpy)) {
		std::cout << "i hande nick" << std::endl;
		// handle set nickname
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
	
		std::cout << "i execute " << fullCommand << std::endl;
	
		CommandManager::redirectCommand(fullCommand, *curUser);
	}
}


void CommandManager::handleNick(std::string command, User &user) {
	size_t start = command.find_first_not_of(" \t\r\n", 4);
	if (start == std::string::npos) {
		//:localhost 431 ${nickname} ${newNickname} :No nickname given
		return;
	}

	size_t end = command.find_first_of(" \t\r\n", start);
	std::string nickname = "";

	if (end != std::string::npos) {
		nickname = command.substr(start, end - start);
	}

	if (nickname.empty()) {
		//:localhost 431 ${nickname} ${newNickname} :No nickname given
		return;
	}

	if (nickname.size() > 9) {
		// need response
		return;
	}

	if (!isValidNickname(nickname)) {
		// :localhost 432 ${nickname} ${nickname} :Nickname is unavailable: Illegal characters
		return;
	}

	user.setNickName(nickname);
	std::cout << "New nickname: " << user.getNickName() << std::endl;
}

bool	CommandManager::isValidNickname(std::string nickname) {
	if (nickname.empty())
		return false;

	// handle special character check


	return true;
}
