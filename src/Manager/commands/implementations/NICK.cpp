#include "../commandManager.hpp"

void CommandManager::handleNick(std::string command, User &user) {
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

	if (server.getUserByNickname(param) != NULL) {
		// nickname already takken;
		return ;
	}

	if (hasForbiddenNickChar(param)) {
		// :localhost 432 ${nickname} ${nickname} :Nickname is unavailable: Illegal characters
		return;
	}

	user.setNickName(param);
	std::cout << "New nickname: <" << user.getNickName() << ">" << std::endl;
	// send succes response
}
