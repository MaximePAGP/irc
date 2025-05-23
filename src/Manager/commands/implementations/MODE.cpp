#include "../CommandManager.hpp"


static	void	addChannelPassord(std::string password, Channel &channel, User &user) {
	if (channel.getChanOpByNickname(user.getNickName()) == NULL) {
		Message::youreNotChanOp(channel.getName(), user);
		return;
	}
	if (password.size() < 1 || password.empty())
		return;

	password = password.substr(1);

	if (password.size() > 12) {
		Message::chanPassordToLong(channel.getName(), user, password);
		return;
	}

	if (Channel::hasForbbidenCharPassword(password)) {
		Message::chanPasswordForbiddenChar(channel.getName(), user, password);
		return;
	}

	std::string flagWithPassword = " +k ";
	flagWithPassword.append(password);
	channel.setPassword(password);

	std::set<User *> usersInChan = channel.getCurrentUsers();
	std::set<User *>::iterator it = usersInChan.begin();
	while (it != usersInChan.end())
	{
		Message::changedModeChan(user.getNickName(), channel.getName(), *(*it), flagWithPassword);
		it++;
	}
}

static	void	removeChannelPassord(std::string uslessArg, Channel &channel, User &user) {
	(void)uslessArg;
	if (channel.getChanOpByNickname(user.getNickName()) == NULL) {
		Message::youreNotChanOp(channel.getName(), user);
		return;
	}

	channel.setPassword("");

	std::set<User *> usersInChan = channel.getCurrentUsers();
	std::set<User *>::iterator it = usersInChan.begin();
	while (it != usersInChan.end())
	{
		Message::changedModeChan(user.getNickName(), channel.getName(), *(*it), " -k");
		it++;
	}
}

static	void	addInvitationOnly(std::string uslessArg, Channel &channel, User &user) {
	(void) uslessArg;
	if (channel.getChanOpByNickname(user.getNickName()) == NULL) {
		Message::youreNotChanOp(channel.getName(), user);
		return;
	}
	if (channel.getIsOnInvitationOnly())
		return;
	channel.setIsOnInvitationOnly(true);
	
	std::set<User *> usersInChan = channel.getCurrentUsers();
	std::set<User *>::iterator it = usersInChan.begin();
	while (it != usersInChan.end())
	{
		Message::changedModeChan(user.getNickName(), channel.getName(), *(*it), " +i");
		it++;
	}
}

static	void	removeInvitationOnly(std::string uslessArg, Channel &channel, User &user) {
	(void) uslessArg;
	if (channel.getChanOpByNickname(user.getNickName()) == NULL) {
		Message::youreNotChanOp(channel.getName(), user);
		return;
	}
	if (!channel.getIsOnInvitationOnly())
		return;
	channel.setIsOnInvitationOnly(false);

	std::set<User *> usersInChan = channel.getCurrentUsers();
	std::set<User *>::iterator it = usersInChan.begin();
	while (it != usersInChan.end())
	{
		Message::changedModeChan(user.getNickName(), channel.getName(), *(*it), " -i");
		it++;
	}
}



static	void	addChanOp(std::string nickname, Channel &channel, User &user) {
	if (channel.getChanOpByNickname(user.getNickName()) == NULL) {
		Message::youreNotChanOp(channel.getName(), user);
		return;
	}

	if (nickname.size() < 1 || nickname.empty())
		return;
	
	nickname = nickname.substr(1);
	
	User *targetUser = channel.getConnectedUserByNickname(nickname);
	if (targetUser == NULL) {
		Message::noSuchNickChannel(nickname, user);
		return;
	}

	channel.addChanOps(*targetUser);

	std::string flagWithTarget = " +o ";
	flagWithTarget.append(targetUser->getNickName());


	std::set<User *> usersInChan = channel.getCurrentUsers();
	std::set<User *>::iterator it = usersInChan.begin();
	while (it != usersInChan.end())
	{
		Message::changedModeChan(user.getNickName(), channel.getName(), *(*it), flagWithTarget);
		it++;
	}
}

static	void	removedChanOp(std::string nickname, Channel &channel, User &user) {
	if (channel.getChanOpByNickname(user.getNickName()) == NULL) {
		Message::youreNotChanOp(channel.getName(), user);
		return;
	}

	if (nickname.size() < 1 || nickname.empty())
		return;

	User *targetUser = channel.getConnectedUserByNickname(nickname);
	if (targetUser == NULL) {
		Message::noSuchNickChannel(nickname, user);
		return;
	}
	std::string flagWithTarget = " -o ";
	flagWithTarget.append(targetUser->getNickName());
	channel.removeChanOps(*targetUser);

	std::set<User *> usersInChan = channel.getCurrentUsers();
	std::set<User *>::iterator it = usersInChan.begin();
	while (it != usersInChan.end())
	{
		Message::changedModeChan(user.getNickName(), channel.getName(), *(*it), flagWithTarget);
		it++;
	}
}



static	void	addLimit(std::string limit, Channel &channel, User &user) {
	if (channel.getChanOpByNickname(user.getNickName()) == NULL) {
		Message::youreNotChanOp(channel.getName(), user);
		return;
	}

	if (limit.size() < 1 || limit.empty())
		return;

	int parsedLimit = atoi(limit.c_str());

	if (parsedLimit < 1 || parsedLimit > 10)
		return;
	std::string flagWithTarget = " +l ";
	std::stringstream convertInt;
 
 	convertInt << parsedLimit;
 		
 	std::string convertValue = convertInt.str();
 	flagWithTarget.append(convertValue);

	channel.setUserlimit(parsedLimit);

	std::set<User *> usersInChan = channel.getCurrentUsers();
	std::set<User *>::iterator it = usersInChan.begin();
	while (it != usersInChan.end())
	{
		Message::changedModeChan(user.getNickName(), channel.getName(), *(*it), flagWithTarget);
		it++;
	}
}

static	void	removeLimit(std::string usless, Channel &channel, User &user) {
	(void)usless;
	if (channel.getChanOpByNickname(user.getNickName()) == NULL) {
		Message::youreNotChanOp(channel.getName(), user);
		return;
	}

	channel.setUserlimit(10);

	std::set<User *> usersInChan = channel.getCurrentUsers();
	std::set<User *>::iterator it = usersInChan.begin();
	while (it != usersInChan.end())
	{
		Message::changedModeChan(user.getNickName(), channel.getName(), *(*it), " -l");
		it++;
	}
}



static	void	addTopicProtection(std::string usless, Channel &channel, User &user) {
	(void)usless;
	if (channel.getChanOpByNickname(user.getNickName()) == NULL) {
		Message::youreNotChanOp(channel.getName(), user);
		return;
	}

	if (!channel.getHasProtectedTopic() == false)
		return;
	channel.setHasProtectedTopic(true);
	
	std::set<User *> usersInChan = channel.getCurrentUsers();
	std::set<User *>::iterator it = usersInChan.begin();
	while (it != usersInChan.end())
	{
		Message::changedModeChan(user.getNickName(), channel.getName(), *(*it), " +t");
		it++;
	}
}

static	void	removeTopicProtection(std::string usless, Channel &channel, User &user) {
	(void)usless;
	if (channel.getChanOpByNickname(user.getNickName()) == NULL) {
		Message::youreNotChanOp(channel.getName(), user);
		return;
	}

	if (!channel.getHasProtectedTopic())
		return;
	channel.setHasProtectedTopic(false);

	std::set<User *> usersInChan = channel.getCurrentUsers();
	std::set<User *>::iterator it = usersInChan.begin();
	while (it != usersInChan.end())
	{
		Message::changedModeChan(user.getNickName(), channel.getName(), *(*it), " -t");
		it++;
	}
}


typedef void (*ActionFunction) (std::string arg, Channel &canal, User &user);


void	loadFunctions(std::map<std::string, ActionFunction> &container) {
	container["+k"] = addChannelPassord;
	container["+i"] = addInvitationOnly;
	container["+t"] = addTopicProtection;
	container["+o"] = addChanOp;
	container["+l"] = addLimit;
	container["-k"] = removeChannelPassord;
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
				result.push_back(tmp[i].append(tmp[i + 1]));
			} else {
				result.push_back(tmp[i]);
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

std::map<std::string, ActionFunction> implementedFlags;

static	void	handleMultiFlags(std::string &command, std::vector<std::string> parsedParams, User &user, Channel &canal) {
	size_t flagIndex = command.find_first_of("+-");
	if (flagIndex == std::string::npos)
		return;
	
	size_t endFlagIndex = command.find_first_of(" ", flagIndex);
	if (endFlagIndex == std::string::npos && command[flagIndex] != '-')
		return;

	std::string flags = command.substr(flagIndex, endFlagIndex);

	if (flags.size() - 1 > parsedParams.size())
		for (size_t i = 0; i < parsedParams.size() - flags.size(); i++)
			parsedParams.push_back("");	

	for (size_t i = 1; i < flags.size(); i++)
	{
		std::string	flag = flags.substr(0, 1);
		flag += flags[i];
		if (implementedFlags.find(flag) != implementedFlags.end())
			implementedFlags[flag](parsedParams[i - 1], canal, user);			
		else
			Message::unknowFlag(user, flag);
	}
}



void CommandManager::handleMode(std::string param, User &user) {
	Server const &server = Server::getServer();
	
	if (param.empty() || param.size() < 2)
		return;

	std::string canalName = CommandManager::trimFirstParamSpace(param);

	if (canalName[0] != '#') {
		Message::modeNotSuchChannel(user, canalName);
		return;
	}
	
	canalName = canalName.substr(1);

	Channel *canal = server.getChannelByName(canalName);

	if (canal == NULL) {
		Message::modeNotSuchChannel(user, canalName);
		return;
	}

	std::string flag = param.substr(param.find_first_of(canalName) + canalName.size());

	if (flag.size() <= 1) {
		canal->sendActiveMode(user);
		return;
	}

	flag = flag.substr(1);

	loadFunctions(implementedFlags);

	if (hasMultipleFlags(flag)) {
		handleMultiFlags(flag, getParams(flag), user, *canal);
		return;
	}
	
	size_t argSep = flag.find_first_of(" ");
	std::string args = "";
	if (argSep != std::string::npos) {
		args = flag.substr(2);
		flag = flag.substr(0, argSep);
	}

	if (implementedFlags.find(flag) == implementedFlags.end()) {
		Message::unknowFlag(user, flag);
		return;
	}
	implementedFlags[flag](args, *canal, user);
}
