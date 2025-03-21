#include <map>
#include "../commandManager.hpp"

// si ya dla merde avant les flags je renvoi une erreur 
	// >> :localhost 472 ${nickname} ${unknowFlag} :is unknown mode char to me

// si ya dla merde apres je prends le flag

static	std::string	getFirstFlagParam(std::string canalName, std::string rawCommand) {
	std::string nextArg = rawCommand.substr(canalName.size() + 1, rawCommand.size());

	if (nextArg.size() == 1) {
		// no args detected so give back flags
		return "";
	}

	std::cout << "next args " << nextArg.size() << " <" << (nextArg == "\n") << ">" << std::endl;

	size_t firstFlagIndex = nextArg.find_first_of("+-");

	if (firstFlagIndex == std::string::npos) {
		// >> :localhost 472 ${nickname} ${unknowFlag} :is unknown mode char to me
		return "";
	}

	for (size_t i = 0; i < firstFlagIndex; i++)
	{
		if (std::isspace(nextArg[i]) == 0) {
			std::cout << "break " << std::string(nextArg, i) <<std::endl;
			// >> :localhost 472 ${nickname} ${unknowFlag} :is unknown mode char to me
			return "";
		}
	}

	return nextArg.substr(firstFlagIndex, 2);
}


static	void	addCanalPassord(std::string password, Canal &canal, User &user) {
	(void)password;
	(void)canal;
	(void)user;
}

static	void	removeCanalPassord(std::string password, Canal &canal, User &user) {
	(void)password;
	(void)canal;
	(void)user;
}


typedef void (*ActionFunction) (std::string password, Canal &canal, User &user);

void CommandManager::handleMode(std::string command, User &user) {
	std::string canalName =  CommandManager::trimFirstParamSpace(command);
	// Server const &server = Server::getServer();

	if (canalName.empty()) {
		// send back active mode on this exact order
			//:localhost 324 ${nickname} ${canalName} +iklt ${canalPassword} ${canalLimit} 
		return;
	}

	// Canal *canal = server.getCanalByName(canalName);

	// if (canal == NULL) {
	// 	// send message to client canal not found
	// 	// :localhost 401 ${nicknanem} ${canalName} :No such nick/channel
	// 	return ;
	// }

	std::string flag = getFirstFlagParam(canalName, command);

	if (flag.size() <= 1)
		return;

   std::map<std::string, ActionFunction> supportedMode;
   supportedMode["+k"] = addCanalPassord;
   supportedMode["-k"] = removeCanalPassord;


	std::cout << "mon petit flag " << flag << std::endl;



	(void)user;
}
