#include "../CommandManager.hpp"

static	void	getCurrentTopic(Channel &canal, User &user) {
	if (canal.getTopic().empty()) {
		Message::topicNoTopic(user, canal.getName());
		return;
    }
	Message::topicGetTopic(user, canal);
}

void	CommandManager::handleTopic(std::string command, User &user) {
	Server &server = Server::getServer();

	if (command.empty()) {
		Message::notEnoughParams(user, "TOPIC");
		return;
	}

	command = command.substr(1);

	size_t nextSpaceJump = command.find_first_of(" ");
	if (nextSpaceJump == std::string::npos) {
		Message::notEnoughParams(user, "TOPIC");
		return;
	}

	std::string canalName = command.substr(0, nextSpaceJump);
	std::string newTopic = command.substr(nextSpaceJump + 1, command.size());
	
	if (newTopic.empty()) {
		Message::notEnoughParams(user, "TOPIC");
		return;
	}
	
	if (canalName[0] != '#') {
		Message::modeNotSuchChannel(user, canalName);
		return;
	}

	canalName = canalName.substr(1);
	Channel *targetCanal = server.getChannelByName(canalName);

	if (newTopic[0] != ':') {
		getCurrentTopic(*targetCanal, user);
		return;
	}

	newTopic = newTopic.substr(1);
	
	if (targetCanal == NULL) {
		Message::modeNotSuchChannel(user, canalName);
		return;
	}

	if (targetCanal->getHasProtectedTopic() && targetCanal->getChanOpByNickname(user.getNickName()) == NULL) {
		Message::youreNotChanOp(targetCanal->getName(), user);
		return;
	}

	targetCanal->setTopic(newTopic);

	std::set<User *> usersInChan = targetCanal->getCurrentUsers();
	std::set<User *>::iterator it = usersInChan.begin();
	while (it != usersInChan.end())
	{
		Message::topicSetTopic(*(*it), user.getNickName(), *targetCanal);
		it++;
	}
}