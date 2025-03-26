#include "../CommandManager.hpp"

void CommandManager::handleUsername(std::string param, User &user) {
	Server const &server = Server::getServer();
	
	if (param.empty() || (param.find(" ") + 1) == param.size()) {
		std::cout << ":localhost 461 " << user.getNickName() << " USER :Not enough parameters" << std::endl;
		return;
	}

	param = param.substr(1, param.size()); // jump space

	if (param.size() > LIMIT_USERNAME_NICKNAME) {
		std::cout << ":localhost 432 " << user.getNickName() << " " << param << " :Erroneous username" << std::endl;		return;
		return;
	}

	if (server.getUserByUsername(param) != NULL) {
		std::cout << ":localhost 433 " << user.getNickName() << " " << param << " :Username is already in use" << std::endl;
		return;
	}

	if (User::hasForbiddenUsernameChar(param)) {
		std::cout << ":localhost 432 " << user.getNickName() << " " << param << " :Username contains forbidden characters" << std::endl;
		return;
	}

	if (!user.getUserName().empty()) {
		std::cout << ":localhost 462 " << user.getNickName() << " :You may not reregister" << std::endl;
		return;
	}

	user.setUsername(param);
	std::cout << ":localhost USER " << user.getUserName() << std::endl;
}
