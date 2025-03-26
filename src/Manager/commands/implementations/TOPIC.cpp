#include "../CommandManager.hpp"

static	void	getCurrentTopic(Canal &canal, User &user) {
	if (canal.getTopic().empty()) {
		std::cout << ":localhost 331 " << user.getNickName() << " #" 
			<< canal.getName() << " :No topic is set" << std::endl;
		return;
    }
	std::cout << ":localhost 332 " << user.getNickName() << " #" 
		<< canal.getName() << " :" << canal.getTopic() << std::endl;
}

std::string	getCanalNameStart(std::string command) {
	if (command.size() < 2)
		return "";
	return command.substr(2, command.size());
}

std::string trimToFirstSpace(std::string command) {
    size_t firstSpaceIndex = command.find(' ');
    if (firstSpaceIndex != std::string::npos){
        return command.substr(0, firstSpaceIndex);
	}
    return command;
}

void	CommandManager::handleTopic(std::string command, User &user) {
	Server &server = Server::getServer();

	if (command.empty())
		return;

	std::string jumpedSpace = getCanalNameStart(command);
	std::string canalName = trimToFirstSpace(jumpedSpace);
	Canal *targetCanal = server.getCanalByName(canalName);

	if (targetCanal == NULL) {
		std::cout << ":localhost 403 " << user.getNickName() <<" "  << command << " :No such channel" << std::endl;
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
	std::string newTopic = command.substr(topicParamIndex + 1, command.size());
	targetCanal->setTopic(newTopic);
	std::cout << ":localhost " << user.getNickName() << " TOPIC #" << targetCanal->getName() << " :" << newTopic << std::endl;
}