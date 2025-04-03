#include "../CommandManager.hpp"


static	void	addCanalPassord(std::string password, Canal &canal, User &user) {
	if (canal.getChanOpByNickname(user.getNickName()) == NULL) {
		Message::youreNotChanOp(canal.getName(), user);
		return;
	}
	if (password.size() <= 1)
		return;

	password = password.substr(1, password.size());

	if (password.size() > 12) {
		Message::chanPassordToLong(canal.getName(), user, password);
		return;
	}

	if (Canal::hasForbbidenCharPassword(password)) {
		Message::chanPasswordForbiddenChar(canal.getName(), user, password);
		return;
	}

	std::string flagWithPassword = " +k ";
	flagWithPassword.append(password);
	Message::changedModeChan(canal.getName(), user, flagWithPassword);
	canal.setPassword(password);
}

static	void	removeCanalPassord(std::string uslessArg, Canal &canal, User &user) {
	(void)uslessArg;
	if (canal.getChanOpByNickname(user.getNickName()) == NULL) {
		Message::youreNotChanOp(canal.getName(), user);
		return;
	}
	Message::changedModeChan(canal.getName(), user, " -k");
	canal.setPassword("");
}



static	void	addInvitationOnly(std::string uslessArg, Canal &canal, User &user) {
	(void) uslessArg;
	if (canal.getChanOpByNickname(user.getNickName()) == NULL) {
		Message::youreNotChanOp(canal.getName(), user);
		return;
	}
	if (canal.getIsOnInvitationOnly())
		return;
	canal.setIsOnInvitationOnly(true);
	Message::changedModeChan(canal.getName(), user, " +i");
}

static	void	removeInvitationOnly(std::string uslessArg, Canal &canal, User &user) {
	(void) uslessArg;
	if (canal.getChanOpByNickname(user.getNickName()) == NULL) {
		Message::youreNotChanOp(canal.getName(), user);
		return;
	}
	if (!canal.getIsOnInvitationOnly())
		return;
	canal.setIsOnInvitationOnly(false);
	Message::changedModeChan(canal.getName(), user, " -i");
}



static	void	addChanOp(std::string nickname, Canal &canal, User &user) {
	if (user.getNickName() == nickname)
		return;
	
	if (canal.getChanOpByNickname(user.getNickName()) == NULL) {
		Message::youreNotChanOp(canal.getName(), user);
		return;
	}

	if (nickname.size() < 1)
		return;
	nickname = nickname.substr(1, nickname.size());
	
	User *targetUser = canal.getConnectedUserByNickname(nickname);
	if (targetUser == NULL) {
		Message::noSuchNickChannel(nickname, user);
		return;
	}

	canal.addChanOps(*targetUser);

	std::string flagWithTarget = " +o ";
	flagWithTarget.append(targetUser->getNickName());
	Message::changedModeChan(canal.getName(), user, flagWithTarget);
}

static	void	removedChanOp(std::string nickname, Canal &canal, User &user) {
	if (canal.getChanOpByNickname(user.getNickName()) == NULL) {
		Message::youreNotChanOp(canal.getName(), user);
		return;
	}

	if (nickname.size() < 1)
		return;
	nickname = nickname.substr(1, nickname.size());

	User *targetUser = canal.getConnectedUserByNickname(nickname);
	if (targetUser == NULL) {
		Message::noSuchNickChannel(nickname, user);
		return;
	}
	std::string flagWithTarget = " -o ";
	flagWithTarget.append(targetUser->getNickName());
	Message::changedModeChan(canal.getName(), user, flagWithTarget);
	canal.removeChanOps(*targetUser);
}



static	void	addLimit(std::string limit, Canal &canal, User &user) {
	if (canal.getChanOpByNickname(user.getNickName()) == NULL) {
		Message::youreNotChanOp(canal.getName(), user);
		return;
	}

	if (limit.size() < 1)
		return;
	limit = limit.substr(1, limit.size());

	int parsedLimit = atoi(limit.c_str());

	if (parsedLimit < 1 || parsedLimit > 10)
		return;
	std::string flagWithTarget = " +l ";
	std::stringstream convertInt;
 
 	convertInt << parsedLimit;
 		
 	std::string convertValue = convertInt.str();
 	flagWithTarget.append(convertValue);

	Message::changedModeChan(canal.getName(), user, flagWithTarget);
	canal.setUserlimit(parsedLimit);
}

static	void	removeLimit(std::string usless, Canal &canal, User &user) {
	(void)usless;
	if (canal.getChanOpByNickname(user.getNickName()) == NULL) {
		Message::youreNotChanOp(canal.getName(), user);
		return;
	}

	canal.setUserlimit(10);
	Message::changedModeChan(canal.getName(), user, " -l");
}



static	void	addTopicProtection(std::string usless, Canal &canal, User &user) {
	(void)usless;
	if (canal.getChanOpByNickname(user.getNickName()) == NULL) {
		Message::youreNotChanOp(canal.getName(), user);
		return;
	}

	if (canal.getHasProtectedTopic() == false) {
		Message::changedModeChan(canal.getName(), user, " +t");
		return;
	}
}

static	void	removeTopicProtection(std::string usless, Canal &canal, User &user) {
	(void)usless;
	if (canal.getChanOpByNickname(user.getNickName()) == NULL) {
		Message::youreNotChanOp(canal.getName(), user);
		return;
	}

	if (canal.getHasProtectedTopic() == true) {
		Message::changedModeChan(canal.getName(), user, " -t");
		return;
	}
}


typedef void (*ActionFunction) (std::string arg, Canal &canal, User &user);


void	loadFunctions(std::map<std::string, ActionFunction> &container) {
	container["+k"] = addCanalPassord;
	container["+i"] = addInvitationOnly;
	container["+t"] = addTopicProtection;
	container["+o"] = addChanOp;
	container["+l"] = addLimit;
	container["-k"] = removeCanalPassord;
	container["-i"] = removeInvitationOnly;
	container["-t"] = removeTopicProtection;
	container["-o"] = removedChanOp;
	container["-l"] = removeLimit;
}


static	std::vector<std::string>	getParams(std::string command) {
	std::vector<std::string> result;
	std::vector<std::string> tmp;

	size_t skipFlagIndex = command.find_first_of(" ");

	if (skipFlagIndex == std::string::npos)
		return result;

	command = command.substr(skipFlagIndex);

	size_t start = 0;
	
	while (start < command.size()) {
		size_t end = command.find_first_of(" ", start);

		if (end == std::string::npos) {
			tmp.push_back(command.substr(start));
			break;
		}
		if (end > start)
			tmp.push_back(command.substr(start, end - start));

		size_t nextStart = command.find_first_not_of(" ", end);
		if (nextStart == std::string::npos) {
			tmp.push_back(command.substr(end));
			break;
		}
		tmp.push_back(command.substr(end, nextStart - end));
		start = nextStart;
	}

	for (size_t i = 0; i < tmp.size(); i++)
	{
		if (i % 2 == 0) {
			if ((i + 1) < tmp.size() && tmp[i].find(" ") != std::string::npos) {
				result.push_back(tmp[i].substr(1).append(tmp[i + 1]));
			} else {
				result.push_back(tmp[i].substr(1));
			}
		}
	}
	
	if (result.size() > 1 && result.back().find_last_not_of(" ") == std::string::npos) {
		std::string spaces = result.back();
		result.pop_back();
		result.back().append(spaces);
	}

	return result;
}


static	bool	hasMultipleFlags(std::string &command) {
	size_t endFlagIndex = command.find_first_of(" ");

	std::string const trimFlag = command.substr(0, endFlagIndex);

	if (trimFlag.size() > 2)
		return true;
	
	return false;
}

void CommandManager::handleMode(std::string param, User &user) {
	Server const &server = Server::getServer();
	
	if (param.empty() || param.size() < 2)
		return;

	std::string canalName = CommandManager::trimFirstParamSpace(param);
	Canal *canal = server.getCanalByName(canalName);

	if (canal == NULL) {
		Message::noSuchNickChannel(canalName, user);
		return;
	}
	std::string flag = param.substr(param.find_first_of(canalName) + canalName.size());
	
	if (flag.size() < 1) {
		canal->sendActiveMode(user);
		return;
	}

	flag = flag.substr(1);

	std::map<std::string, ActionFunction> implementedFlags;

	loadFunctions(implementedFlags);
	if (hasMultipleFlags(flag)) {
		getParams(flag);
		return;
	}

	if (implementedFlags.find(flag) == implementedFlags.end()) {
		Message::unknowFlag(user, flag);
		return;
	}
	
	std::string skipFlag = param.substr(param.find(flag) + flag.size());
	implementedFlags[flag](skipFlag, *canal, user);
}
