#include "../CommandManager.hpp"

void CommandManager::handleInvite(std::string param, User &user)
{
    if (param.empty()) {
        Message::notEnoughParams(user, "INVITE");
        return;
    }

    Server &server = Server::getServer();
    std::string targetUserName, channelName;

    param = param.substr(1);

    size_t spaceSep = param.find_first_of(" ");
    if (spaceSep == std::string::npos) {
        Message::notEnoughParams(user, "INVITE");
        return;
    }

    targetUserName = param.substr(0, spaceSep);
    channelName = param.substr(spaceSep + 1);

    if (channelName.empty()) {
        Message::notEnoughParams(user, "INVITE");
        return;
    }

    channelName = channelName.substr(1);

    User* targetUser = server.getUserByNickname(targetUserName);
    if (!targetUser) {
        Message::noSuchNickChannel(targetUserName, user);
        return;
    }

    Channel* channel = server.getChannelByName(channelName);
    if (!channel) {
        Message::noSuchNickChannel(channelName, user);
        return;
    }

    if (!channel->getConnectedUserByNickname(user.getNickName())) {
        Message::modeNotSuchChannel(user, channelName);
        return;
    }

    if (channel->getIsOnInvitationOnly() && !channel->getChanOpByNickname(user.getNickName())) {
        Message::youreNotChanOp(channelName, user);
        return;
    }

    if (channel->getConnectedUserByNickname(targetUser->getNickName())) {
        Message::alreadyOnChannel(user, channelName);
        return;
    }

    channel->addUserInvitation(*targetUser);

    std::string inviteMsg = ":" + user.getNickName() + " INVITE " + targetUserName + " :" + channelName + "\r\n";

    send(targetUser->getFd().fd, inviteMsg.c_str(), inviteMsg.length(), 0);

    std::string confirmMsg = ":server 341 " + user.getNickName() + " " + targetUserName + " " + channelName + "\r\n";
    send(user.getFd().fd, confirmMsg.c_str(), confirmMsg.length(), 0);
}
