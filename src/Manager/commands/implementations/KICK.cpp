#include "../CommandManager.hpp"

void CommandManager::handleKick(std::string param, User &user)
{
	if (param.empty() || param.size() < 2) {
		Message::kickNotEnoughParams(user);
		return;
	}
    
	Server &server = Server::getServer();
	param = param.substr(1);
    std::istringstream iss(param);
    std::string channelName, targetNickname, reason;
	std::string canalName = param.substr(0, param.find_first_of(" "));
	std::string target = param.substr(param.find_first_of(" "));



	std::cout << "param (" << param << ") channeNae (" << canalName << ") reason (" << target << ")" << std::endl;
	Canal *canal = server.getCanalByName(canalName);

	if (!canal) {
		Message::noSuchNickChannel(canalName, user);
		return;
	}

    if (!(iss >> channelName >> targetNickname)) {
        Message::userNoParam(user, "KICK");
        return;
    }

    if (iss.peek() == ' ')
        iss.ignore();
    std::getline(iss, reason);
    if (!reason.empty() && reason[0] == ':')
        reason.erase(0, 1);

    Canal* channel = server.getCanalByName(channelName);
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

    User* targetUser = channel->getConnectedUserByNickname(targetNickname);
    if (!targetUser) {
        Message::noSuchNickChannel(targetNickname, user);
        return;
    }

    channel->removeUser(*targetUser);

    std::string kickMessage = ":" + user.getNickName() + " KICK #" + channelName + " " + targetNickname;
    if (!reason.empty())
        kickMessage += " :" + reason;
    kickMessage += "\r\n";

    send(targetUser->getFd().fd, kickMessage.c_str(), kickMessage.size(), 0);

    std::set<User*> usersInChannel = channel->getCurrentUsers();
    for (std::set<User*>::iterator it = usersInChannel.begin(); it != usersInChannel.end(); ++it) {
        if ((*it)->getNickName() != targetNickname)
            send((*it)->getFd().fd, kickMessage.c_str(), kickMessage.size(), 0);
    }
}

