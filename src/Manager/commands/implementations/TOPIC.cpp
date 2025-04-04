#include "../CommandManager.hpp"

static	void	getCurrentTopic(Canal &canal, User &user) {
	if (canal.getTopic().empty()) {
		Message::topicNoTopic(user, canal.getName());
		return;
    }
	Message::topicGetTopic(user, canal);
}

std::string	getCanalNameStart(std::string command) {
	if (command.size() < 2)
		return "";
	return command.substr(2);
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
		Message::modeNotSuchChannel(user, canalName);
		return;
	}

	size_t	topicParamIndex = command.find_first_of(":"); 

	if (topicParamIndex == std::string::npos) {
		getCurrentTopic(*targetCanal, user);
		return;
	}

	if (targetCanal->getHasProtectedTopic() && targetCanal->getChanOpByNickname(user.getNickName()) == NULL) {
		Message::youreNotChanOp(targetCanal->getName(), user);
	}
	std::string newTopic = command.substr(topicParamIndex + 1, command.size());
	targetCanal->setTopic(newTopic);
	Message::topicSetTopic(user, *targetCanal);
}