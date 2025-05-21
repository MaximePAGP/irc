/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leye <leye@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 02:15:32 by leye              #+#    #+#             */
/*   Updated: 2025/05/21 02:33:01 by rgrangeo         ###   ########.fr       */
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
    if (canal.isProtectedByPassword() && password != canal.getPassword()) 
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
	if (canal.getIsOnInvitationOnly() && !canal.getInvitationUserByUsername(user.getNickName()))
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

void CommandManager::handleJoin(std::string command, User &user) 
{
    if (command.empty()) 
    {
        Message::notEnoughParams(user, "JOIN");
        return;
    }
    
    Server &server = Server::getServer();
    std::string chanalName = "";
    std::string password = "";

    command = command.substr(1);
    size_t sep = command.find_first_of(" ");
    
    if (sep == std::string::npos) 
    {
        chanalName = command;
    } else {
        chanalName = command.substr(0, sep);
        password = command.substr(sep);
    }

    if (chanalName[0] != '#') 
    {
        Message::modeNotSuchChannel(user, chanalName);
        return;
    }
	chanalName = chanalName.substr(1);
    
    if (hasIllegalName(chanalName)) 
    {
        Message::modeNotSuchChannel(user, chanalName);
        return;
    }

    Channel *canal = server.getChannelByName(chanalName);

    if (canal == NULL) 
    {
        canal = new Channel(chanalName);
       server.addCanal(*canal);
        canal->addChanOps(user);
    }

    // Validation des restrictions du canal
    if (!validateChannelRestrictions(*canal, user, password)) 
    {
        return;
    }

    // Ajout de l'utilisateur au canal
    canal->addUser(user);

    // Envoi de la confirmation de JOIN
    std::string joinResponse = ":" + user.getNickName() + "!~" + user.getUserName() + "@localhost JOIN #" + chanalName + "\r\n";
    send(user.getFd().fd, joinResponse.c_str(), joinResponse.length(), 0);

    // Envoi du topic du canal
    if (!canal->getTopic().empty()) 
    {
        std::string topicResponse = ":server 332 " + user.getNickName() + " #" + chanalName + " :" + canal->getTopic() + "\r\n";
        send(user.getFd().fd, topicResponse.c_str(), topicResponse.length(), 0);
    }

    // Envoi de la liste des utilisateurs
    std::string namesResponse = ":server 353 " + user.getNickName() + " = #" + chanalName + " :";
    std::set<User*> channelUsers = canal->getCurrentUsers();
    std::set<User*> chanOps = canal->getChanOps();

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

    // Envoi du message de fin de liste
    std::string endNamesResponse = ":server 366 " + user.getNickName() + " #" + chanalName + " :End of /NAMES list.\r\n";
    send(user.getFd().fd, endNamesResponse.c_str(), endNamesResponse.length(), 0);

    // Notification aux autres utilisateurs
    sendToChannelUsers(channelUsers, joinResponse, &user);

    std::cout << "User " << user.getNickName() << " joined canal #" << chanalName << std::endl;
}
