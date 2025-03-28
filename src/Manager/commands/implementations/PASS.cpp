#include "../CommandManager.hpp"
#include "../../../Class/Server/Server.hpp"

void	CommandManager::handlePass(std::string command, User &user) {
	Server const &server = Server::getServer();

	if (user.getIsConnected() == true)
	{
		send(user.getFd().fd, ":server 462 * :You may not reregister\r\n", 39, 0);
	}
	if (command.find("PASS") != 0)
	{
		send(user.getFd().fd, ":server 464 rgrangeo :Password required\r\n", 41, 0);
		return ;
	}
	command = command.substr(5, command.size());
	if (command == server.getPassword())
	{
		user.setIsConnected(true);
	}
	else
	{
		send(user.getFd().fd, ":server 464 * :Password incorrect\r\n", 35, 0);
		return ;
	}
}
