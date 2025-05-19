#include "../CommandManager.hpp"

static	void	findAndQuitChannels(User &user, std::string const &channelName, std::string const &userLeft, std::string const reason) {
	Server &server = Server::getServer();
	Channel *channel = server.getChannelByName(channelName);

	if (!channel) {
		Message::modeNotSuchChannel(user, channelName);
		return;
	}

	if (!channel->getConnectedUserByNickname(user.getNickName())) {
		Message::partYoureNotInChan(user, channelName);
		return;
	}

	channel->removeUser(user);
	std::set<User *>::iterator it = channel->getCurrentUsers().begin();
	while (it != channel->getCurrentUsers().end())
	{
		Message::partNotification(user, channelName, userLeft, reason);
		it++;
	}
	
}

void	CommandManager::handlePart(std::string param, User &user) {
	if (param.empty()) {
		Message::notEnoughParams(user, "PART");
		return;
	}

	Server &server = Server::getServer();
	(void) server;
	std::string channalName;

	param = param.substr(1);

	while(!param.empty()) {
		size_t sep = param.find_first_of(",");
		if (sep == std::string::npos) {
			channalName = param;
		} else {
			channalName = param.substr(0, sep);
		}
		param = param.substr(channalName.length());
		if (channalName[0] != '#') {
			Message::modeNotSuchChannel(user, channalName);
			continue;
		}
		channalName = channalName.substr(1);

	}
}