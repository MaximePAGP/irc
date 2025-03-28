#include "CommandManager.hpp"

CommandManager::CommandManager() {}


CommandManager::~CommandManager() {}

/*
	Detect if passing command has space before reach to the first alphanumeric value
	Acutally the method do a little bit more, we also looking if the passing command
	has less then 4 and more then 7 size, it is our minimal and maximal handled command
*/
bool CommandManager::hasLeadingSpaces(std::string command) {
	int	const minimalCommandSize = 4;
	int	const maximalCommandSize = 7;

	size_t firstSpaceIndex = command.find_first_of(" ");

	if (firstSpaceIndex == std::string::npos)
		return false;

	if (firstSpaceIndex < minimalCommandSize || firstSpaceIndex > maximalCommandSize) {
		return true;
	}

	return false;
}



typedef void (*commandsModel) (std::string command, User &user);

void	CommandManager::redirectCommand(std::string command, User &user) {
	if (command.empty())
		return;

	if (CommandManager::hasLeadingSpaces(command)) {
		std::cout << ":localhost 421 " << user.getUserName() << " " << command << " :Unknown command" << std::endl;
		return;
	}
	std::string getFirstCommand = getCommand(command);
	
	std::map<std::string, commandsModel> commands;

	if (user.getIsConnected() == false || command.find("PASS") == 0) {
		CommandManager::handlePass(command, user);
		return ;
	}
	
	commands["MODE"] = CommandManager::handleMode;
	commands["JOIN"] = CommandManager::handleJoin;
	commands["TOPIC"] = CommandManager::handleTopic;
	commands["NICK"] = CommandManager::handleNick;
	commands["USER"] = CommandManager::handleUsername;

	if (commands.find(getFirstCommand) == commands.end()) {
		std::cout << ":localhost 421 " << user.getUserName() << " " << command << " :Unknown command" << std::endl;
		return;
	}

	commands[getFirstCommand](command.substr(getFirstCommand.size(), command.size()), user);
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


std::string	CommandManager::getCommand(std::string command) {
	size_t nextSpaceIndex = command.find(" ");

	if (nextSpaceIndex == std::string::npos)
		return command;

	return command.substr(0, nextSpaceIndex);
}

std::string CommandManager::trimFirstParamSpace(std::string param) {
	std::string	jumpSpace = param.substr(2, param.size());

	size_t nextSpaceIndex = jumpSpace.find(" ");

	return jumpSpace.substr(0, nextSpaceIndex);
}

