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

	if (!user.getNickName().empty() && !user.getUserName().empty()) {
        // Envoyer les messages de bienvenue
        std::string welcome = ":server 001 " + user.getNickName() + " :Welcome to the IRC Network " 
                    + user.getNickName() + "!~" + user.getUserName() + "@localhost\r\n";
        send(user.getFd().fd, welcome.c_str(), welcome.length(), 0);
        
        std::string yourhost = ":server 002 " + user.getNickName() + " :Your host is server, running version 1.0\r\n";
        send(user.getFd().fd, yourhost.c_str(), yourhost.length(), 0);
        
        std::string created = ":server 003 " + user.getNickName() + " :This server was created today\r\n";
        send(user.getFd().fd, created.c_str(), created.length(), 0);
        
        std::string myinfo = ":server 004 " + user.getNickName() + " server 1.0 o mt\r\n";
        send(user.getFd().fd, myinfo.c_str(), myinfo.length(), 0);
        
        std::cout << "User " << user.getNickName() << " (" << user.getUserName() << ") registered successfully" << std::endl;
    }
}
