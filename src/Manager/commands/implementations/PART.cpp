#include "../CommandManager.hpp"

static	void	reachNextComma(std::string &chanName) {
	size_t nextCommaIndex = chanName.find_first_of(",");
	if (nextCommaIndex == std::string::npos) {
		chanName.clear();
		return;
	}
	chanName = chanName.substr(nextCommaIndex + 1);
}

static	void	removeReason(std::string &chanName) {
	size_t	reasonSep = chanName.find_first_of(":");
	if (reasonSep == std::string::npos)
		return;
	
	chanName = chanName.substr(0, reasonSep);

	size_t	spaceReasonSep = chanName.find_first_of(" ");
	if (spaceReasonSep == std::string::npos)
		return;
	chanName = chanName.substr(0, spaceReasonSep);
}

static	void	findAndQuitChannels(User &user, std::string const &channelName) {
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

	std::set<User *> usersInChan = channel->getCurrentUsers();
	std::set<User *>::iterator it = usersInChan.begin();
	while (it != usersInChan.end())
	{
		Message::partNotification(*(*it), channelName, user.getNickName());
		it++;
	}
	channel->removeUser(user);
	if (channel->getCurrentUsers().size() > 0)
		return;
	server.removeCanal(*channel);
	delete channel;
}

void	CommandManager::handlePart(std::string param, User &user) {
	if (param.empty()) {
		Message::notEnoughParams(user, "PART");
		return;
	}

	Server &server = Server::getServer();
	(void) server;
	std::string channelName;

	param = param.substr(1);
	int i = 0;
	while(param.empty() == false && i < 50) {
		i++;
		size_t sep = param.find_first_of(",");
		if (sep == std::string::npos) {
			channelName = param;
		} else {
			channelName = param.substr(0, sep);
		}
		param = param.substr(channelName.length());
		if (channelName[0] != '#') {
			Message::modeNotSuchChannel(user, channelName);
			reachNextComma(param);
			continue;
		}
		channelName = channelName.substr(1);
		removeReason(channelName);
		findAndQuitChannels(user, channelName);
	}
}