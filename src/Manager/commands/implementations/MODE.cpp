#include "../commandManager.hpp"


void CommandManager::handleMode(std::string command, User &user) {
	std::string param =  CommandManager::trimParamSpace(command);
	Server const &server = Server::getServer();

	if (param.empty()) {
		// send back active mode on this exact order
			//:localhost 324 ${nickname} ${canalName} +iklt ${canalPassword} ${canalLimit} 

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
