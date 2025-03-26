#include "../CommandManager.hpp"

void CommandManager::handleNick(std::string param, User &user) {
	Server const &server = Server::getServer();

	if (param.empty() || (param.find(" ") + 1) == param.size()) {
		std::cout << ":localhost 431 " << user.getUserName() << " " << param << " :No nickname given" << std::endl;
		return;
	}

	param = param.substr(1, param.size()); // jump space

	if (param.size() > LIMIT_USERNAME_NICKNAME) {
		std::cout << ":localhost 432 " << user.getUserName() << " " << param << " :Erroneous nickname" << std::endl;
		return;
	}

	if (server.getUserByNickname(param) != NULL) {
		std::cout << ":localhost 433 " << user.getUserName() << " " << param << " :Nickname is already in use" << std::endl;
		return ;
	}

	if (User::hasForbiddenNickChar(param)) {
		std::cout << ":localhost 432 " << user.getUserName() << " " << param << " :Nickname is unavailable: Illegal characters" << std::endl;
		return;
	}

	user.setNickName(param);
	std::cout << ":localhost NICK " << user.getNickName() << std::endl;
}
