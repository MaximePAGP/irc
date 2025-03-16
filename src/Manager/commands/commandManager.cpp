#include "commandManager.hpp"

CommandManager::CommandManager() {}


CommandManager::~CommandManager() {}


/*

	La taille minimum acceptable est 4 car la plus petite commande valide en taille est 4 (KICK ou MODE)

*/
bool	CommandManager::hasValidCommand(std::string command) {
	int	const validCommandSize = 5;
	size_t const firstSpaceIndex = command.find(" ");

	if (firstSpaceIndex == std::string::npos)
		return false;

	if (validCommandSize > firstSpaceIndex)
		return false;

	for (size_t i = 0; i < firstSpaceIndex; i++)
	{
		if (!std::isalpha(command[i]))
			return false;
	}
	
	return true;
}

bool	CommandManager::isNick(std::string command) {
	size_t test = command.find("nick");
	std::cout << " result   "  << test << " " << command << std::endl;
	if (command.find("nick") == 5){
		return true;
	}
	return false;
}

void	CommandManager::redirectCommand(std::string command) {
	if (command.empty())
		return ;
	std::string cmdCpy = command;
	std::transform(cmdCpy.begin(), cmdCpy.end(), cmdCpy.begin(), ::tolower);

	if (!this->hasValidCommand(cmdCpy)) {
		// handle response here
			//should response to client :localhost 421 salut {command} :Unknown command
		return ;
	}

	if (this->isNick(cmdCpy)) {

		// handle set nickname
	}

	// if no handle case return same as 
		//should response to client :localhost 421 salut {command} :Unknown command
}