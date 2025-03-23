#include "../commandManager.hpp"
#include "../../../Class/Server/Server.hpp"

void	CommandManager::handlePass(std::string command, User &user) {
	Server const &server = Server::getServer();
	std::string param = CommandManager::trimParamSpace(command);

	if (command.find("PASS") != 0)
	{
		send(user.getFd().fd, ":server 464 rgrangeo :Password required\r\n", 41, 0);
		return ;
	}
	if (param == server.getPassword())
	{
		user.setIsConnected(true);
	}
	else
	{
		send(user.getFd().fd, ":server 464 * :Password incorrect\r\n", 38, 0);
		return ;
	}
}
