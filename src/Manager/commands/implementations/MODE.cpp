#include <map>
#include "../commandManager.hpp"

// si ya dla merde avant les flags je renvoi une erreur 
	// >> :localhost 472 ${nickname} ${unknowFlag} :is unknown mode char to me

// si ya dla merde apres je prends le flag

static	std::string	getFirstFlagParam(std::string canalName, std::string rawCommand) {
	std::string rightSideArg = rawCommand.substr(canalName.size() + 2, rawCommand.size());

	if (rightSideArg.size() == 1) {
		// no args detected so give back flags
		return "";
	}

	size_t firstFlagIndex = rightSideArg.find_first_of("+-");

	if (firstFlagIndex == std::string::npos) {
		// >> :localhost 472 ${nickname} ${unknowFlag} :is unknown mode char to me
		return "";
	}

	for (size_t i = 0; i < firstFlagIndex; i++) // check if there is only space between canalname and the first flag
	{
		if (std::isspace(rightSideArg[i]) == 0) {
			std::cout << "break " << std::string(rightSideArg, i) <<std::endl;
			// >> :localhost 472 ${nickname} ${unknowFlag} :is unknown mode char to me
			return "";
		}
	}

	return rightSideArg.substr(firstFlagIndex, 2);
}


static	void	addCanalPassord(std::string password, Canal &canal, User &user) {
	if (canal.getChanOpByNickname(user.getNickName()) == NULL) {
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
		// >> :localhost 482 username[ #canalName :You're not channel operator
		return;
	}
	std::cout << "jai eneleve" << std::endl;
	canal.setPassword("");
}


static	void	addInvitationOnly(std::string uslessArg, Canal &canal, User &user) {
	(void) uslessArg;
	if (canal.getChanOpByNickname(user.getNickName()) == NULL) {
		// >> :localhost 482 username[ #canalName :You're not channel operator
		return;
	}
	if (canal.getIsOnInvitationOnly())
		return;
	canal.setIsOnInvitationOnly(true);
	// :localhost username MODE #canalName +i
	std::cout << "jai set a +i" << std::endl;
}


static	void	removeInvitationOnly(std::string uslessArg, Canal &canal, User &user) {
	(void) uslessArg;
	if (canal.getChanOpByNickname(user.getNickName()) == NULL) {
		// >> :localhost 482 username[ #canalName :You're not channel operator
		return;
	}
	if (!canal.getIsOnInvitationOnly())
		return;
	canal.setIsOnInvitationOnly(false);
	// :localhost username MODE #canalName -i
	std::cout << "jai set a -i" << std::endl;
}


static	void	addChanOp(std::string targetUsername, Canal &canal, User &user) {
	if (canal.getChanOpByNickname(user.getNickName()) == NULL) {
		// >> :localhost 482 username[ #canalName :You're not channel operator
		return;
	}

	if (targetUsername.empty())
		return;
	
	User *targetUser = canal.getConnectedUserByNickname(targetUsername);
	if (targetUser == NULL) {
		// :localhost 401 ElitE-40912 , :No such nick/channel
		return;
	}

	canal.addChanOps(*targetUser);
	std::cout << "i add chan op from " << targetUsername << std::endl;
 	// :localhost MODE #canalName +o targetName
}


static	void	removedChanOp(std::string targetUsername, Canal &canal, User &user) {
	if (canal.getChanOpByNickname(user.getNickName()) == NULL) {
		// >> :localhost 482 username[ #canalName :You're not channel operator
		return;
	}

	if (targetUsername.empty())
		return;

	User *targetUser = canal.getConnectedUserByNickname(targetUsername);
	std::cout << *targetUser;
	if (targetUser == NULL) {
		// :localhost 401 ElitE-40912 , :No such nick/channel
		return;
	}

	canal.removeChanOps(*targetUser);
	std::cout << "i removed chan op from " << targetUsername << std::endl;
 	// :localhost MODE #canalName -o targetName
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
	canalName = canalName.substr(1, canalName.size()); // remove # or & from canalName
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
	supportedMode["+i"] = addInvitationOnly;
	supportedMode["-i"] = removeInvitationOnly;
	supportedMode["+o"] = addChanOp;
	supportedMode["-o"] = removedChanOp;

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
