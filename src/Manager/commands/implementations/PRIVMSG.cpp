/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leye <leye@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:06:40 by leye              #+#    #+#             */
/*   Updated: 2025/04/10 20:33:12 by leye             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../CommandManager.hpp"
#include "../../../Class/Server/Server.hpp"

void CommandManager::handlePrivmsg(std::string command, User &user) {
    // Vérifiez si l'utilisateur est enregistré
    if (user.getNickName().empty() || user.getUserName().empty()) {
        Message::noSuchCommand(user, "PRIVMSG");
        return;
    }

    // Extraire la cible et le message
    size_t spacePos = command.find(' ');
    if (spacePos == std::string::npos) {
        Message::noSuchCommand(user, "PRIVMSG");
        return;
    }

    std::string target = command.substr(0, spacePos);
    size_t msgStart = command.find(':', spacePos);
    if (msgStart == std::string::npos) {
        Message::noSuchCommand(user, "PRIVMSG");
        return;
    }

    std::string message = command.substr(msgStart + 1);

    // Obtenir le serveur
    Server &server = Server::getServer();

    // Si la cible est un canal
    if (target[0] == '#') {
        Canal *canal = server.getCanalByName(target);
        if (!canal) {
            Message::noSuchNickChannel(target, user);
            return;
        }
        // Vérifiez si l'utilisateur est dans le canal
        if (canal->getConnectedUserByNickname(user.getNickName()) == NULL) {
            Message::noSuchNickChannel(target, user);
            return;
        }
        // Envoyer le message à tous les utilisateurs du canal
        std::set<User*> channelUsers = canal->getCurrentUsers();
        for (std::set<User*>::iterator it = channelUsers.begin(); it != channelUsers.end(); ++it) {
            if ((*it)->getNickName() != user.getNickName()) {
                std::string msgToSend = ":" + user.getNickName() + " PRIVMSG " + target + " :" + message + "\r\n";
                send((*it)->getFd().fd, msgToSend.c_str(), msgToSend.length(), 0);
            }
        }
    }
    // Si la cible est un utilisateur
    else {
        User *targetUser = server.getUserByNickname(target);
        if (!targetUser) {
            Message::noSuchNickChannel(target, user);
            return;
        }
        // Envoyer le message à l'utilisateur cible
        std::string msgToSend = ":" + user.getNickName() + " PRIVMSG " + target + " :" + message + "\r\n";
        send(targetUser->getFd().fd, msgToSend.c_str(), msgToSend.length(), 0);
    }
}