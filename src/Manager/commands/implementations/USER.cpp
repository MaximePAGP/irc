#include "../CommandManager.hpp"

void CommandManager::handleUsername(std::string param, User &user) {
	Server const &server = Server::getServer();
	
	if (param.empty() || param.size() < 2) {
		Message::userNoParam(user, param);
		return;
	}

	param = param.substr(1);
	size_t trimCommand = param.find(" ");

	if (trimCommand != std::string::npos)
		param = param.substr(0, trimCommand);
	
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
