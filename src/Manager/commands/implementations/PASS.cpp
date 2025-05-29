#include "../CommandManager.hpp"
#include "../../../Class/Server/Server.hpp"

void	CommandManager::handlePass(std::string command, User &user) {
	Server const &server = Server::getServer();
	std::string msg = "";

	if (command.empty())
	{
		msg = ":localhost 461 * PASS :Not enough parameters\r\n";
		Server::psend(user.getFd().fd, msg.c_str(), msg.length(), 0);
		return ;
	}
	if (user.getIsConnected() == true)
	{
		msg = ":localhost 462 * :You may not reregister\r\n";
		Server::psend(user.getFd().fd, msg.c_str(), msg.length(), 0);
		return ;
	}
	if (command.find("PASS") != 0)
	{
		msg = ":localhost 464 * :Password required\r\n";
		Server::psend(user.getFd().fd, msg.c_str(), msg.length(), 0);
		return ;
	}
	command = command.substr(getCommand(command).size(), command.size());
	if (command.empty())
	{
		Message::notEnoughParams(user, "PASS");
		return ;
	}
	command = CommandManager::trimFirstParamSpace(command);
	if (command == server.getPassword())
	{
		user.setIsConnected(true);
	}
	else
	{
		msg = ":localhost 464 * :Password incorrect\r\n";
		Server::psend(user.getFd().fd, msg.c_str(), msg.length(), 0);
		return ;
	}
}
