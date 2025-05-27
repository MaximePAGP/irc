#include "../CommandManager.hpp"

void CommandManager::handleUsername(std::string param, User &user) {
	Server const &server = Server::getServer();
	
	if (param.empty() || param.size() < 2) {
		Message::notEnoughParams(user, "USER");
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

	if (User::hasForbiddenUsernameChar(param)) {
		Message::userForbiddenChar(user, param);
		return;
	}

	if (server.getUserByUsername(param) != NULL) {
		Message::userAlreadyTaken(user, param);
		return;
	}

	if (!user.getUserName().empty()) {
		Message::userCannotChange(user, param);
		return;
	}

	user.setUsername(param);
	Message::userSet(user);

	if (!user.getNickName().empty() && !user.getUserName().empty()) {	
        // Envoyer les messages de bienvenue
        std::string welcome = ":localhost 001 " + user.getNickName() + " :Welcome to the IRC Network " 
                    + user.getNickName() + "!~" + user.getUserName() + "@localhost\r\n";
        Server::psend(user.getFd().fd, welcome.c_str(), welcome.length(), 0);
        
        std::string yourhost = ":localhost 002 " + user.getNickName() + " :Your host is server, running version 1.0\r\n";
        Server::psend(user.getFd().fd, yourhost.c_str(), yourhost.length(), 0);
        
        std::string created = ":localhost 003 " + user.getNickName() + " :This server was created today\r\n";
        Server::psend(user.getFd().fd, created.c_str(), created.length(), 0);
        
        std::string myinfo = ":localhost 004 " + user.getNickName() + " server 1.0 o mt\r\n";
        Server::psend(user.getFd().fd, myinfo.c_str(), myinfo.length(), 0);
    }
}
