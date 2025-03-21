#include "../commandManager.hpp"
#include "../../../Class/Server/Server.hpp"

void	CommandManager::handlePass(std::string command, User &user) {
	if (command.find("PASS") != 0)
		send(user.getFd().fd, ":server 464 rgrangeo :Password required\r\n", 37, 0);
}
