#include "../CommandManager.hpp"

void CommandManager::handleUsername(std::string param, User &user) {
	Server const &server = Server::getServer();
	
	if (param.empty() || (param.find(" ") + 1) == param.size()) {
		Message::userNoParam(user, param);
		return;
	}

	param = param.substr(1, param.size()); // jump space

	size_t trimCommand = param.find("0 ");

	if (trimCommand != std::string::npos)
		param = param.substr(0, trimCommand - 1);
	
	if (param.size() > LIMIT_USERNAME_NICKNAME) {
		Message::userToLong(user, param);
		return;
	}

	if (server.getUserByUsername(param) != NULL) {
		Message::userAlreadyTaken(user, param);
		return;
	}

	if (User::hasForbiddenUsernameChar(param)) {
		Message::userForbiddenChar(user, param);
		return;
	}

	if (!user.getUserName().empty()) {
		Message::userCannotChange(user, param);
		return;
	}

	user.setUsername(param);
	Message::userSet(user);
}
