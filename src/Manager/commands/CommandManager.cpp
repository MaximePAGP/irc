#include "CommandManager.hpp"

CommandManager::CommandManager() {}


CommandManager::~CommandManager() {}

/*
	Detect if passing command has space before reach to the first alphanumeric value
	Acutally the method do a little bit more, we also looking if the passing command
	has less then 4 and more then 7 size, it is our minimal and maximal handled command
*/
bool CommandManager::hasLeadingSpaces(std::string command) {
	int	const minimalCommandSize = 3;
	int	const maximalCommandSize = 7;

	size_t firstSpaceIndex = command.find_first_of(" ");

	if (firstSpaceIndex == std::string::npos)
		return false;

	if (firstSpaceIndex < minimalCommandSize || firstSpaceIndex > maximalCommandSize) {
		return true;
	}

	return false; }

typedef void (*commandsModel) (std::string command, User &user);

void	CommandManager::redirectCommand(std::string command, User &user) {
	if (command.empty())
		return;

	if (CommandManager::hasLeadingSpaces(command)) {
		Message::noSuchCommand(user, command);
		return;
	}

	std::string getFirstCommand = getCommand(command);
	std::map<std::string, commandsModel> commands;

	commands["MODE"] = CommandManager::handleMode;
	commands["JOIN"] = CommandManager::handleJoin;
	commands["TOPIC"] = CommandManager::handleTopic;
	commands["KICK"] = CommandManager::handleKick;
	commands["INVITE"] = CommandManager::handleInvite;
	commands["PRIVMSG"] = CommandManager::handlePrivmsg;
	commands["PART"] = CommandManager::handlePart;
	commands["GPT"] = CommandManager::handleGpt;
	commands["NICK"] = CommandManager::handleNick;
	commands["USER"] = CommandManager::handleUsername;
	commands["PASS"] = CommandManager::handlePass;

	if (user.getIsConnected() == false || getFirstCommand == "PASS") {
		CommandManager::handlePass(command, user);
		return;
	}
	
	if (getFirstCommand != "NICK" && getFirstCommand != "USER") {
		if ((user.getNickName() == "" || user.getUserName() == "")) {
			Message::noRegistered(user);
			return;
		}
	}
	
	if (commands.find(getFirstCommand) == commands.end()) {
		Message::noSuchCommand(user, command);
		return;
	}

	commands[getFirstCommand](command.substr(getFirstCommand.size(), command.size()), user);
}


void	CommandManager::buildCommand(std::string command, int clientFd) {
	Server const &server = Server::getServer();
	User *curUser = server.getUserByFd(clientFd);
	
	if ((curUser->getCommandBuffer().size() + command.size()) > MSG_LEN) {
		Message::commandToLong(*curUser);
		curUser->flushCommandBuffer();
		return;
	}
	curUser->setCommandBuffer(
		curUser->getCommandBuffer().append(command)
	);

	while (curUser->getCommandBuffer().find(END_CMD) != std::string::npos) {
		size_t pos = curUser->getCommandBuffer().find(END_CMD);
	
		std::string fullCommand = curUser->getCommandBuffer().substr(0, pos);
		curUser->setCommandBuffer(curUser->getCommandBuffer().substr(pos + 2, curUser->getCommandBuffer().size()));
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
	std::string	jumpSpace = param.substr(1, param.size());

	size_t nextSpaceIndex = jumpSpace.find(" ");

	return jumpSpace.substr(0, nextSpaceIndex);
}
