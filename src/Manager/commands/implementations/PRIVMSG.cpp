/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leye <leye@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:06:40 by leye              #+#    #+#             */
/*   Updated: 2025/04/24 06:21:37 by leye             ###   ########.fr       */
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
    command = command.substr(1); // Enlever le premier caractère ':'
    std::cout << "Command: " << command << std::endl;
    // Vérifiez si la commande est vide
    if (command.empty()) {
        Message::noSuchCommand(user, "PRIVMSG");
        return;
    }
    // Extraire la cible = 1er paramètre de command
    size_t firstSpace = command.find(' ');
    if (firstSpace == std::string::npos) {
        Message::noSuchCommand(user, "PRIVMSG");
        return;
    }
    std::string target = command.substr(0, firstSpace);
    if (target.empty()) {
        Message::noSuchCommand(user, "PRIVMSG");
        return;
    }
    std::cout << "Target: " << target << std::endl;
    // Extraire le message = 2ème paramètre de command et tout le reste
    std::string message = command.substr(firstSpace + 1);
    if (message.empty()) {
        Message::noSuchCommand(user, "PRIVMSG");
        return;
    }
    std::cout << "Message: " << message << std::endl;
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
        // Envoyer le message à tous les utilisateurs du canal sauf l'expéditeur
        std::set<User*> channelUsers = canal->getCurrentUsers();
        for (std::set<User*>::iterator it = channelUsers.begin(); it != channelUsers.end(); ++it)
        {
            if (*it == &user) {
                continue; // Exclure l'utilisateur qui envoie le message
            }
            std::cout << "Sending message to user: " << (*it)->getNickName() << std::endl;
            std::string msgToSend = ":" + user.getNickName() + " PRIVMSG " + target + " :" + message + "\r\n";
            send((*it)->getFd().fd, msgToSend.c_str(), msgToSend.length(), 0);
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