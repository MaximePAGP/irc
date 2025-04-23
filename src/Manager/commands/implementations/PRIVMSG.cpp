/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leye <leye@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:06:40 by leye              #+#    #+#             */
/*   Updated: 2025/04/14 19:29:18 by leye             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../CommandManager.hpp"
#include "../../../Class/Server/Server.hpp"

void sendMessageToUser(User &sender, User &receiver, const std::string &message) {
    std::string msgToSend = ":" + sender.getNickName() + " PRIVMSG " + receiver.getNickName() + " :" + message + "\r\n";
    send(receiver.getFd().fd, msgToSend.c_str(), msgToSend.length(), 0);
}

void sendMessageToChannel(User &sender, Canal &channel, const std::string &message) {
    std::string msgToSend = ":" + sender.getNickName() + " PRIVMSG #" + channel.getName() + " :" + message + "\r\n";
    for (std::set<User*>::iterator it = channel.getCurrentUsers().begin(); it != channel.getCurrentUsers().end(); ++it) {
        send((*it)->getFd().fd, msgToSend.c_str(), msgToSend.length(), 0);
    }
}

void CommandManager::handlePrivmsg(std::string command, User &user) {
    if (user.getNickName().empty() || user.getUserName().empty()) {
        Message::noSuchCommand(user, "PRIVMSG");
        return;
    }

    command = command.substr(1); // Enlever le premier caractère ':'
    size_t firstSpace = command.find(' ');
    if (firstSpace == std::string::npos) {
        Message::noSuchCommand(user, "PRIVMSG");
        return;
    }

    std::string target = command.substr(0, firstSpace);
    std::string message = command.substr(firstSpace + 1);
    if (target.empty() || message.empty()) {
        Message::noSuchCommand(user, "PRIVMSG");
        return;
    }
    if (message.length() > MSG_LEN) {
        std::cerr << "Message too long" << std::endl;
        return;
    }

    Server &server = Server::getServer();
    if (target[0] == '#') {
        Canal *canal = server.getCanalByName(target);
        if (!canal || canal->getConnectedUserByNickname(user.getNickName()) == NULL) {
            Message::noSuchNickChannel(target, user);
            return;
        }
        sendMessageToChannel(user, *canal, message);
    } else {
        User *targetUser = server.getUserByNickname(target);
        if (!targetUser) {
            Message::noSuchNickChannel(target, user);
            return;
        }
        sendMessageToUser(user, *targetUser, message);
    }
}