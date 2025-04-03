#include "../CommandManager.hpp"

void CommandManager::handleNick(std::string param, User &user) {
	Server const &server = Server::getServer();

	if (param.empty() || (param.find(" ") + 1) == param.size()) {
		Message::nickNoParam(user, param);
		return;
	}

	param = param.substr(1);

	if (param.size() > LIMIT_USERNAME_NICKNAME) {
		Message::nickToLongParam(user, param);
		return;
	}

	if (server.getUserByNickname(param) != NULL) {
		Message::nickAlreadyUsed(user, param);
		return ;
	}

	if (User::hasForbiddenNickChar(param)) {
		Message::nickIllegal(user, param);
		return;
	}
	if (user.getNickName().empty()) {
		user.setNickName(param);
		Message::nickSet(user);
		return;
	}
	Message::nickSetUpdated(user, param);
	user.setNickName(param);
}
