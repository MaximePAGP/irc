#include "../CommandManager.hpp"


static	void	addCanalPassord(std::string password, Canal &canal, User &user) {
	if (canal.getChanOpByNickname(user.getNickName())) {
		std::cout << ":localhost 482 " << user.getNickName() << " #" << canal.getName()
			<< " :You're not channel operator" << std::endl;
		return;
	}
	if (password.size() <= 1)
		return;

	password = password.substr(1, password.size());

	if (password.size() > 12) {
		std::cout << ":localhost 696 " << user.getNickName() << " #" << canal.getName()
			<< " +k " << password << " :Channel key is too long" << std::endl;
		return;
	}

	if (Canal::hasForbbidenCharPassword(password)) {
		std::cout << ":localhost 696 " << user.getNickName() << " #" << canal.getName()
			<< " +k " << password
			<< " :Channel key contains forbidden characters" << std::endl;
		return;
	}

	canal.setPassword(password);
	std::cout << ":localhost 324 " << user.getNickName() << " " << canal.getName()
	<< " +k " << password << std::endl;}

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
	if (canal.getChanOpByNickname(user.getNickName())) {
		std::cout << "Youre not chan op" << std::endl;
		return;
	}
	if (canal.getIsOnInvitationOnly())
		return;
	canal.setIsOnInvitationOnly(true);
	std::cout << ":localhost " << user.getNickName() << " MODE #" << canal.getName() << " +i" << std::endl;
}

static	void	removeInvitationOnly(std::string uslessArg, Canal &canal, User &user) {
	(void) uslessArg;
	if (canal.getChanOpByNickname(user.getNickName())) {
		// >> :localhost 482 username[ #canalName :You're not channel operator
		return;
	}
	if (!canal.getIsOnInvitationOnly())
		return;
	canal.setIsOnInvitationOnly(false);
	std::cout << ":localhost " << user.getNickName() << " MODE #" << canal.getName() << " -i" << std::endl;
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
		std::cout << ":localhost 401 " << user.getNickName() << " " << targetUsername 
		<< " :No such nick/channel" << std::endl;
		return;
	}

	canal.addChanOps(*targetUser);
	std::cout << ":localhost " << user.getNickName() << " MODE +o " << targetUser << std::endl;
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



static	void	addLimit(std::string limit, Canal &canal, User &user) {
	if (canal.getChanOpByNickname(user.getNickName())) {
		// >> :localhost 482 username[ #canalName :You're not channel operator
		return;
	}

	if (limit.empty())
		return;

	int parsedLimit = atoi(limit.c_str());

	if (parsedLimit < 1 || parsedLimit > 10)
		return;
	std::cout << "limit has been set to " << parsedLimit << std::endl;
	canal.setUserlimit(parsedLimit);
}

static	void	removeLimit(std::string usless, Canal &canal, User &user) {
	(void)usless;
	if (canal.getChanOpByNickname(user.getNickName())) {
		// >> :localhost 482 username[ #canalName :You're not channel operator
		return;
	}

	// >> :localhost MODE #canalName -l 
	canal.setUserlimit(10);
	std::cout << "limit of canal has been reset to 10" << std::endl;
}



static	void	addTopicProtection(std::string usless, Canal &canal, User &user) {
	(void)usless;
	if (canal.getChanOpByNickname(user.getNickName())) {
		std::cout << ":localhost 482 " << user.getNickName() << " #" << canal.getName() << " :You're not channel operator" << std::endl;
		return;
	}

	if (canal.getHasProtectedTopic() == false) {
		std::cout << ":localhost MODE #" << canal.getName() << " +t" << std::endl;
		return;
	}
}

static	void	removeTopicProtection(std::string usless, Canal &canal, User &user) {
	(void)usless;
	if (canal.getChanOpByNickname(user.getNickName())) {
		std::cout << ":localhost 482 " << user.getNickName() << " #" << canal.getName() << " :You're not channel operator" << std::endl;
		return;
	}

	if (canal.getHasProtectedTopic() == true) {
		std::cout << ":localhost MODE #" << canal.getName() << " -t" << std::endl;
		return;
	}
}



typedef void (*ActionFunction) (std::string arg, Canal &canal, User &user);

void CommandManager::handleMode(std::string param, User &user) {
	Server const &server = Server::getServer();
	
	if (param.empty() || param.size() < 2)
		return;
	
	std::string canalName = CommandManager::trimFirstParamSpace(param);

	Canal *canal = server.getCanalByName(canalName);

	if (canal == NULL) {
		std::cout << ":localhost 401 " << user.getNickName() << " " << canalName 
		<< " :No such nick/channel" << std::endl;
		return;
	}
	std::string flag = param.substr(param.find_first_of(canalName) + canalName.size(), param.size());
	
	if (flag.size() < 1) {
		std::cout << "Get back flag" << std::endl;
		return;
	}

	flag = flag.substr(1, 2); // get the two first one after space

	std::map<std::string, ActionFunction> implementedFlags;

	implementedFlags["+k"] = addCanalPassord;
	implementedFlags["-k"] = removeCanalPassord;
	implementedFlags["+i"] = addInvitationOnly;
	implementedFlags["-i"] = removeInvitationOnly;
	implementedFlags["+o"] = addChanOp;
	implementedFlags["-o"] = removedChanOp;
	implementedFlags["+l"] = addLimit;
	implementedFlags["-l"] = removeLimit;
	implementedFlags["+t"] = addTopicProtection;
	implementedFlags["-t"] = removeTopicProtection;

	if (implementedFlags.find(flag) == implementedFlags.end()) {
		std::cout << ":localhost 472 " << user.getNickName() << " " << flag 
			<< " :is unknown mode char to me" << std::endl;
		return;
	}
	
	std::string skipFlag = param.substr(param.find_first_of(flag) + flag.size(), param.size());

	implementedFlags[flag](skipFlag, *canal, user);
}
