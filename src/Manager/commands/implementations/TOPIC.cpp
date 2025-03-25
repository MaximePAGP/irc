#include "../CommandManager.hpp"

static	void	getCurrentTopic(Canal &canal, User &user) {
	(void)user;
	if (canal.getTopic().empty()) {
		std::cout << ":localhost 331 nickname #" << canal.getName() <<  " :No topic is set" << std::endl;
		return;
	}
	std::cout << ":localhost 332 nickname #canalname :" << canal.getTopic() << std::endl;
}

static	std::string	getCanalName(std::string command) {
	size_t canalNameIndex = command.find_first_of("#&");

	if (canalNameIndex == std::string::npos)
		return "";
	
	size_t spaceAfterArg = command.substr(canalNameIndex + 1, command.size()).find(" ");
	
	if (spaceAfterArg == std::string::npos)
		return command.substr(canalNameIndex + 1, command.size());
	return command.substr(canalNameIndex + 1, spaceAfterArg);
}

void	CommandManager::handleTopic(std::string command, User &user) {
	Server 			&server = Server::getServer();
	std::string		canalArgName = getCanalName(command);
	Canal 			*targetCanal = server.getCanalByName(canalArgName);


	if (targetCanal == NULL) {
		std::cout << ":localhost 403 username #" << canalArgName << ":No such channel" << std::endl;
		return;
	}

	size_t	topicParamIndex = command.find_first_of(":"); 

	if (topicParamIndex == std::string::npos) {
		getCurrentTopic(*targetCanal, user);
		return;
	}

	if (targetCanal->getHasProtectedTopic() && targetCanal->getChanOpByNickname(user.getNickName()) == NULL) {
		std::cout << ":localhost 482 " << user.getNickName() << " #" << targetCanal->getName() << " :You're not channel operator" << std::endl;
		return;
	}
	std::string newTopic = command.substr(topicParamIndex, command.size());
	std::cout << ":localhost " << user.getNickName() << " TOPIC #" << targetCanal->getName() << " :" << newTopic << std::endl;
}