#include "../commandManager.hpp"

void CommandManager::handleUsername(std::string command, User &user) {
	std::string param =  CommandManager::trimFirstParamSpace(command);
	Server const &server = Server::getServer();

	if (param.empty()) {
		//:localhost 431 ${nickname} ${newNickname} :No nickname given
		return;
	}

	if (param.size() > LIMIT_USERNAME_NICKNAME) {
		// need response
		return;
	}

	if (server.getUserByUsername(param) != NULL) {
		// nickname already takken;
		return ;
	}

	if (hasForbiddenUsernameChar(param)) {
		// :localhost 432 ${nickname} ${nickname} :Nickname is unavailable: Illegal characters
		return;
	}

	if (user.getUserName().size() != 0) {
		// :localhost 462 ${username} :You may not reregisterw
		return;
	}

	user.setUsername(param);
	std::cout << "New UserName: <" << user.getUserName() << ">" << std::endl;
	// send succes response
}
