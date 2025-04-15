#include "../CommandManager.hpp"

void	CommandManager::handleKick(std::string param, User &user)
{
	Server const &server = Server::getServer();
	std::string canalName = CommandManager::trimFirstParamSpace(param);
	Canal *canal = server.getCanalByName(canalName);
	
	std::cout << "Param = " << canalName << std::endl;

	if (canal == NULL)
	{
		Message::modeNotSuchChannel(user, canalName);
		return ;
	}
	if (canal->getChanOpByNickname(user.getNickName()) == NULL)
		Message::youreNotChanOp(canal->getName(), user);


	//User user2 = user;

	
}
