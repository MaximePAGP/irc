#include "../CommandManager.hpp"

void	CommandManager::handleKick(std::string param, User &user)
{
	Server const &server = Server::getServer();
	std::string canalName = CommandManager::trimFirstParamSpace(param);
	Canal *canal = server.getCanalByName(canalName);
	
	if (canal == NULL)
	{
		//user.send_message("test\r\n");
		std::cout << "Error" << std::endl;
		return ;
	}
	if (canal->getChanOpByNickname(user.getNickName()) == NULL)
		std::cout << "No" << std::endl;


	//User user2 = user;

	
	std::cout << "Param = " << param << std::endl;
}
