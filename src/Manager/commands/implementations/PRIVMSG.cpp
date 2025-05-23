/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leye <leye@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:06:40 by leye              #+#    #+#             */
/*   Updated: 2025/05/23 02:08:31 by rgrangeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../CommandManager.hpp"
#include "../../../Class/Server/Server.hpp"

void CommandManager::handlePrivmsg(std::string command, User &user) 
{

	if (command.empty() || command.size() <= 2) 
	{
		Message::notEnoughParams(user, "PRIVMSG");
		return;
	}
	command = command.substr(1);
	size_t firstSpace = command.find(' ');
	
	if (firstSpace == std::string::npos) 
	{
		Message::notEnoughParams(user, "PRIVMSG");
		return;
	}
	
	std::string target = command.substr(0, firstSpace);
	
	if (target.empty()) 
	{
		Message::notEnoughParams(user, "PRIVMSG");
		return;
	}
	// Extraire le message = 2ème paramètre de command et tout le reste
	std::string message = command.substr(firstSpace + 1);

	if (message.empty()) 
	{
		Message::noSuchCommand(user, "PRIVMSG");
		return;
	}
	Server &server = Server::getServer();
	
	if (target[0] == '#') 
	{
		target = target.substr(1);
		Channel *canal = server.getChannelByName(target);
		
		if (!canal) 
		{
			Message::noSuchNickChannel(target, user);
			return;
		}
		
		if (canal->getConnectedUserByNickname(user.getNickName()) == NULL) 
		{
			Message::noSuchNickChannel(target, user);
			return;
		}
		
		std::set<User*> channelUsers = canal->getCurrentUsers();
		for (std::set<User*>::iterator it = channelUsers.begin(); it != channelUsers.end(); ++it)
		{
			if ((*it)->getNickName() == user.getNickName()) 
			{
				continue;
			}
			
			std::string msgToSend = ":" + user.getNickName() + " PRIVMSG #" + target + " " + message + END_CMD;
			send((*it)->getFd().fd, msgToSend.c_str(), msgToSend.length(), 0);
		}
	} else {
		
		User *targetUser = server.getUserByNickname(target);
		
		if (!targetUser) 
		{
			Message::noSuchNickChannel(target, user);
			return;
		}

		std::string msgToSend = ":";
		msgToSend.append(user.getNickName());
		msgToSend.append(" PRIVMSG ");
		msgToSend.append(target);
		msgToSend.append(" ");
		msgToSend.append(message);
		msgToSend.append(END_CMD);

		send(targetUser->getFd().fd, msgToSend.c_str(), msgToSend.length(), 0);
	}
}
