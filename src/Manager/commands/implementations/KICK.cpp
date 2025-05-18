#include "../CommandManager.hpp"

void CommandManager::handleKick(std::string param, User &user)
{
    Server &server = Server::getServer();
    Message messageManager;
    std::istringstream iss(param);
    std::string channelName, targetNickname, reason;

    if (!(iss >> channelName >> targetNickname)) {
        messageManager.userNoParam(user, "KICK");
        return;
    }

    if (iss.peek() == ' ')
        iss.ignore();
    std::getline(iss, reason);
    if (!reason.empty() && reason[0] == ':')
        reason.erase(0, 1);

    Canal* channel = server.getCanalByName(channelName);
    if (!channel) {
        messageManager.modeNotSuchChannel(user, channelName);
        return;
    }

    if (!channel->getConnectedUserByNickname(user.getNickName())) {
        messageManager.youreNotChanOp(channelName, user);
        return;
    }

    if (!channel->getChanOpByNickname(user.getNickName())) {
        messageManager.youreNotChanOp(channelName, user);
        return;
    }

    User* targetUser = channel->getConnectedUserByNickname(targetNickname);
    if (!targetUser) {
        messageManager.noSuchNickChannel(targetNickname, user);
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

