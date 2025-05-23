/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magrondi <magrondi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 02:15:32 by leye              #+#    #+#             */
/*   Updated: 2025/05/23 13:49:27 by magrondi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../CommandManager.hpp"

bool	canalHasFlag(bool hasPasswordParam, Channel &canal, User &user) 
{
	(void)user;
	if (hasPasswordParam == false && canal.getPassword().size() > 1) 
    {
		// :localhost 475 nickname canalName :Cannot join channel (+k)
		return true;
	}
	if (canal.getUserLimits() < static_cast<int>(canal.getCurrentUsers().size()) + 1) 
    {
		// :localhost 471 nickname canalName :Cannot join channel (+l)
		return true;
	}
	if (canal.getIsOnInvitationOnly()) 
    {
		// :localhost 473 nickname canalName :Cannot join channel (+i)
		return true;
	}
	return false;
}

void sendToChannelUsers(const std::set<User*> &users, const std::string &message, User *excludeUser = NULL) 
{
    for (std::set<User*>::iterator it = users.begin(); it != users.end(); ++it) 
    {
        if (*it == excludeUser) 
        {
            continue; // Exclure l'utilisateur émetteur
        }
        send((*it)->getFd().fd, message.c_str(), message.length(), 0);
    }
}

// Fonction pour valider les restrictions du canal
bool validateChannelRestrictions(Channel &canal, User &user, const std::string &password) 
{
    User *invitation = canal.getInvitationUserByUsername(user.getNickName());
    if (canal.isProtectedByPassword() && password != canal.getPassword() && !invitation) 
    {
        std::string errorMsg = ":server 475 " + user.getNickName() + " " + canal.getName() + " :Cannot join channel (+k) - bad key\r\n";
        send(user.getFd().fd, errorMsg.c_str(), errorMsg.length(), 0);
        return false;
    }   
    if (canal.getUserLimits() <= static_cast<int>(canal.getCurrentUsers().size())) 
    {
        std::string errorMsg = ":server 471 " + user.getNickName() + " " + canal.getName() + " :Cannot join channel (+l) - channel is full\r\n";
        send(user.getFd().fd, errorMsg.c_str(), errorMsg.length(), 0);
        return false;
    }
	if (canal.getIsOnInvitationOnly() && !invitation)
    {
        std::string errorMsg = ":server 473 " + user.getNickName() + " " + canal.getName() + " :Cannot join channel (+i) - invite only\r\n";
        send(user.getFd().fd, errorMsg.c_str(), errorMsg.length(), 0);
        return false;
    }
    return true;
}

static	bool	hasIllegalName(std::string const &name) 
{
	if (name.size() > MAX_NAME_LENGTH)
		return true;
	if (name.find_first_of(" \0:,") != std::string::npos)
		return true;
	return false;
}

static void splitByDelimiter(const std::string &input, char delimiter, std::vector<std::string> &output)
{
    size_t start = 0, end;
    while ((end = input.find(delimiter, start)) != std::string::npos)
    {  
        output.push_back(input.substr(start, end - start));
        start = end + 1;
    }
    if (start < input.length())
    {
        output.push_back(input.substr(start));
    }
}

static void extractChannelsAndPasswords(const std::string &command, std::vector<std::string> &channels, std::vector<std::string> &passwords)
{
    size_t spacePos = command.find(' ');

    std::string channelsPart =  (spacePos != std::string::npos) ? command.substr(1, spacePos - 1) : command.substr(1, command.size());
    std::string passwordsPart = (spacePos != std::string::npos) ? command.substr(spacePos + 1) : "";

    splitByDelimiter(channelsPart, ',', channels);
    splitByDelimiter(passwordsPart, ',', passwords);
}

static bool isValidChannelName(std::string &channelName, User &user)
{
    if (channelName[0] != '#')
    {
        Message::modeNotSuchChannel(user, channelName);
        return false;
    }
    channelName = channelName.substr(1);

    if (hasIllegalName(channelName))
    {
        Message::modeNotSuchChannel(user, channelName);
        return false;
    }
    return true;
}

Channel *createNewChannel(std::string &channelName, User &user, Server &server)
{
    Channel *channel = new Channel(channelName);
    server.addCanal(*channel);
    channel->addChanOps(user);
    return channel;
}

void sendUserList(Channel &channel, User &user)
{
    std::string namesResponse = ":server 353 " + user.getNickName() + " = #" + channel.getName() + " :";
    std::set<User*> channelUsers = channel.getCurrentUsers();
    std::set<User*> chanOps = channel.getChanOps();

    for (std::set<User*>::iterator it = channelUsers.begin(); it != channelUsers.end(); ++it)
    {
        User *channelUser = *it;
        if (chanOps.find(channelUser) != chanOps.end())
        {
            namesResponse += "@";
        }
        namesResponse += channelUser->getNickName() + " ";
    }
    namesResponse += "\r\n";
    send(user.getFd().fd, namesResponse.c_str(), namesResponse.length(), 0);

    std::string endNamesResponse = ":server 366 " + user.getNickName() + " #" + channel.getName() + " :End of /NAMES list.\r\n";
    send(user.getFd().fd, endNamesResponse.c_str(), endNamesResponse.length(), 0);
}
void joinChannel(Channel &channel, User &user)
{
    channel.addUser(user);
    channel.removeUserInvitation(user);

    std::string joinResponse = ":" + user.getNickName() + "!~" + user.getUserName() + "@localhost JOIN #" + channel.getName() + "\r\n";
    send(user.getFd().fd, joinResponse.c_str(), joinResponse.length(), 0);

    if (!channel.getTopic().empty())
    {
        std::string topicResponse = ":server 332 " + user.getNickName() + " #" + channel.getName() + " :" + channel.getTopic() + "\r\n";
        send(user.getFd().fd, topicResponse.c_str(), topicResponse.length(), 0);
    }

    sendUserList(channel, user);
    sendToChannelUsers(channel.getCurrentUsers(), joinResponse, &user);
}


void CommandManager::handleJoin(std::string command, User &user)
{
    if (command.empty())
    {
        Message::notEnoughParams(user, "JOIN");
        return;
    }
    Server &server = Server::getServer();
    
    std::vector<std::string> channels;
    std::vector<std::string> passwords;
    extractChannelsAndPasswords(command, channels, passwords);
    
    if (channels.size() > 10) {
        Message::joinToMuchChan(user, command.data());
        return;
    }

    for (size_t i = 0; i < channels.size(); ++i)
    {
        std::string channelName = channels[i];
        std::string password = (i < passwords.size()) ? passwords[i] : "";
        if (channelName.size() <= 1) {
            Message::notEnoughParams(user, channelName);
            continue;
        }
        if (!isValidChannelName(channelName, user))
            continue;
        Channel *channel = server.getChannelByName(channelName);
       if (!channel)
       {
            channel = createNewChannel(channelName, user, server);
       }
       if (!validateChannelRestrictions(*channel, user, password))
            continue;

        joinChannel(*channel, user);
    }
}