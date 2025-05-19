#include "../CommandManager.hpp"


static	void	extractParams(std::string rawParams, std::string &canalName, std::string &targetName, std::string &reason) {
	reason = "";
	targetName = "";
	canalName = "";

	size_t canalNameSepPos = rawParams.find_first_of(" ");

	if (canalNameSepPos == std::string::npos) {
		canalName = rawParams;
		return;
	}
	
	canalName = rawParams.substr(0, canalNameSepPos);

	rawParams = rawParams.substr(canalNameSepPos);

	if (rawParams.empty())
		return;

	rawParams = rawParams.substr(1);
		
	size_t targetSepPos = rawParams.find_first_of(" ");

	if (std::string::npos == targetSepPos) {
		targetName = rawParams;
		return;
	}

	targetName = rawParams.substr(0, targetSepPos);

	rawParams = rawParams.substr(targetSepPos);

	rawParams = rawParams.substr(1);

	if (rawParams.empty())
		return;

	reason = rawParams;
}


void CommandManager::handleKick(std::string param, User &user)
{
	if (param.empty()) {
		Message::notEnoughParams(user, "KICK");
		return;
	}
    
	Server &server = Server::getServer();

	param = param.substr(1);

    std::string channelName, target, reason = "";

	extractParams(param, channelName, target, reason);

	if (channelName.empty() || target.empty()) {
		Message::notEnoughParams(user, "KICK");
		return;
	}

	if (channelName[0] != '#') {
		Message::noSuchNickChannel(channelName, user);
		return;
	}
	channelName = channelName.substr(1);
	std::cout << "param (" << reason << ") channeNae (" << channelName << ") reason (" << target << ")" << std::endl;
	Channel *channel = server.getChannelByName(channelName);

    if (!channel) {
        Message::modeNotSuchChannel(user, channelName);
        return;
    }

    if (!channel->getConnectedUserByNickname(user.getNickName())) {
        Message::youreNotChanOp(channelName, user);
        return;
    }

    if (!channel->getChanOpByNickname(user.getNickName())) {
        Message::youreNotChanOp(channelName, user);
        return;
    }

    User* targetUser = channel->getConnectedUserByNickname(target);
    if (!targetUser) {
        Message::noSuchNickChannel(target, user);
        return;
    }

    std::set<User*> usersInChannel = channel->getCurrentUsers();
    for (std::set<User*>::iterator it = usersInChannel.begin(); it != usersInChannel.end(); ++it) {
            Message::kickSucces(*(*it), *channel, target, reason);
    }
    channel->removeUser(*targetUser);
}
