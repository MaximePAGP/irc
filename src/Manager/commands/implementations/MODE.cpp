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
	if (canal.getChanOpByNickname(user.getNickName()) == NULL) {
		std::cout << "je suis call " << password << std::endl;
		// >> :localhost 482 username[ #canalName :You're not channel operator
		return;
	}

	if (password.size() == 0) // do nothing
		return;

	if (password.size() > 12 || Canal::hasForbbidenCharPassword(password)) {
		//:localhost 696 max{ #super k * :Channel key contains forbidden characters or is too long
		return;
	}
	canal.setPassword(password);
	std::cout << "jai bien set " << password << std::endl;
}

static	void	removeCanalPassord(std::string uslessArg, Canal &canal, User &user) {
	(void)uslessArg;
	if (canal.getChanOpByNickname(user.getNickName()) == NULL) {
		std::cout << "je suis call " << uslessArg << std::endl;
		// >> :localhost 482 username[ #canalName :You're not channel operator
		return;
	}

	canal.setPassword("");
}


typedef void (*ActionFunction) (std::string arg, Canal &canal, User &user);

void CommandManager::handleMode(std::string command, User &user) {
	std::string canalName =  CommandManager::trimFirstParamSpace(command);
	Server const &server = Server::getServer();

	if (canalName.empty()) {
		// send back active mode on this exact order
			//:localhost 324 ${nickname} ${canalName} +iklt ${canalPassword} ${canalLimit} 
		return;
	}

	Canal *canal = server.getCanalByName(canalName);

	if (canal == NULL) {
		// send message to client canal not found
		// :localhost 401 ${nicknanem} ${canalName} :No such nick/channel
		return ;
	}

	std::string flag = getFirstFlagParam(canalName, command);

	if (flag.size() <= 1)
		return;

   std::map<std::string, ActionFunction> supportedMode;
   supportedMode["+k"] = addCanalPassord;
   supportedMode["-k"] = removeCanalPassord;

	if (supportedMode.find(flag) == supportedMode.end()) {
		std::cout << "ya rien frero " << std::endl;
		return;
	}

	std::string skipFlag = command.substr(command.find(flag) + flag.size(), command.size());
	std::string argFlag = CommandManager::trimFirstParamSpace(skipFlag);

	supportedMode[flag](argFlag, *canal, user);
	// send back to client >> :localhsot 472 ${username} ${flag} :is unknown mode char to me
	(void)user;
}
