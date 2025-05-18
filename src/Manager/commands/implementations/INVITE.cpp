#include "../CommandManager.hpp"

void CommandManager::handleInvite(std::string param, User &user)
{
    Server &server = Server::getServer();
    Message messageManager;
    std::istringstream iss(param);
    std::string targetUserName, channelName;

    if (!(iss >> targetUserName >> channelName)) {
        messageManager.notEnoughParams(user, "INVITE");
        return;
    }

    User* targetUser = server.getUserByNickname(targetUserName);
    if (!targetUser) {
        messageManager.noSuchNickChannel(targetUserName, user);
        return;
    }

    Canal* channel = server.getCanalByName(channelName);
    if (!channel) {
        messageManager.noSuchNickChannel(channelName, user);
        return;
    }

    if (!channel->getConnectedUserByNickname(user.getNickName())) {
        messageManager.modeNotSuchChannel(user, channelName);
        return;
    }

    if (channel->getIsOnInvitationOnly() && !channel->getChanOpByNickname(user.getNickName())) {
        messageManager.youreNotChanOp(channelName, user);
        return;
    }

    if (channel->getConnectedUserByNickname(targetUser->getNickName())) {
        messageManager.alreadyOnChannel(user, channelName);
        return;
    }

	channel->addUserInvitation(*targetUser);

    std::string inviteMsg = ":" + user.getNickName() + " INVITE " + targetUserName + " :" + channelName + "\r\n";

    send(targetUser->getFd().fd, inviteMsg.c_str(), inviteMsg.length(), 0);

    std::string confirmMsg = ":server 341 " + user.getNickName() + " " + targetUserName + " " + channelName + "\r\n";
    send(user.getFd().fd, confirmMsg.c_str(), confirmMsg.length(), 0);
} 
