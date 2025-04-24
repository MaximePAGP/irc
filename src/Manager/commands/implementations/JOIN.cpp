/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leye <leye@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 02:15:32 by leye              #+#    #+#             */
/*   Updated: 2025/04/24 06:00:03 by leye             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../CommandManager.hpp"


bool	canalHasFlag(bool hasPasswordParam, Canal &canal, User &user) {
	(void)user;
	if (hasPasswordParam == false && canal.getPassword().size() > 1) {
		// :localhost 475 nickname canalName :Cannot join channel (+k)
		return true;
	}
	if (canal.getUserLimits() < static_cast<int>(canal.getCurrentUsers().size()) + 1) {
		// :localhost 471 nickname canalName :Cannot join channel (+l)
		return true;
	}
	if (canal.getIsOnInvitationOnly()) {
		// :localhost 473 nickname canalName :Cannot join channel (+i)
		return true;
	}
	return false;
}

// Fonction utilitaire pour extraire le mot de passe
std::string extractPassword(const std::string &command) {
    size_t spacePos = command.find(' ');
    if (spacePos != std::string::npos) {
        std::string password = command.substr(spacePos + 1);
        size_t pwEndPos = password.find_first_of(" \r\n");
        if (pwEndPos != std::string::npos) {
            password = password.substr(0, pwEndPos);
        }
        return password;
    }
    return "";
}

void sendToChannelUsers(const std::set<User*> &users, const std::string &message, User *excludeUser = NULL) {
    for (std::set<User*>::iterator it = users.begin(); it != users.end(); ++it) {
        if (*it == excludeUser) {
            continue; // Exclure l'utilisateur émetteur
        }
        send((*it)->getFd().fd, message.c_str(), message.length(), 0);
    }
}
// Fonction pour valider les restrictions du canal
bool validateChannelRestrictions(Canal &canal, User &user, const std::string &password) {
    if (canal.isProtectedByPassword() && password != canal.getPassword()) {
        std::string errorMsg = ":server 475 " + user.getNickName() + " " + canal.getName() + " :Cannot join channel (+k) - bad key\r\n";
        send(user.getFd().fd, errorMsg.c_str(), errorMsg.length(), 0);
        return false;
    }
    if (canal.getUserLimits() <= static_cast<int>(canal.getCurrentUsers().size())) {
        std::string errorMsg = ":server 471 " + user.getNickName() + " " + canal.getName() + " :Cannot join channel (+l) - channel is full\r\n";
        send(user.getFd().fd, errorMsg.c_str(), errorMsg.length(), 0);
        return false;
    }
    if (canal.getIsOnInvitationOnly() && canal.getUserInvitation().find(&user) == canal.getUserInvitation().end()) {
        std::string errorMsg = ":server 473 " + user.getNickName() + " " + canal.getName() + " :Cannot join channel (+i) - invite only\r\n";
        send(user.getFd().fd, errorMsg.c_str(), errorMsg.length(), 0);
        return false;
    }
    return true;
 }

void CommandManager::handleJoin(std::string command, User &user) 
{
    Server &server = Server::getServer();

    // Extraction du nom du canal
    std::string canalName = command.substr(1, command.find(' ') - 1);
    if (canalName.empty() || canalName[0] != '#') {
        std::cerr << "Invalid channel name" << std::endl;
        return;
    }
    if (canalName.length() > 200) {
        std::cerr << "Channel name too long" << std::endl;
        return;
    }

    // Extraction du mot de passe
    std::string password = extractPassword(command);

    // Recherche ou création du canal
    Canal *canal = server.getCanalByName(canalName);
    if (canal == NULL) {
        canal = new Canal(canalName);
        server.addCanal(*canal);
        canal->addChanOps(user);
        std::cout << "Canal " << canalName << " created." << std::endl;
    }

    // Validation des restrictions du canal
    if (!validateChannelRestrictions(*canal, user, password)) {
        return;
    }

    // Ajout de l'utilisateur au canal
    canal->addUser(user);

    // Envoi de la confirmation de JOIN
    std::string joinResponse = ":" + user.getNickName() + "!~" + user.getUserName() + "@localhost JOIN " + canalName + "\r\n";
    send(user.getFd().fd, joinResponse.c_str(), joinResponse.length(), 0);

    // Envoi du topic du canal
    if (!canal->getTopic().empty()) {
        std::string topicResponse = ":server 332 " + user.getNickName() + " " + canalName + " :" + canal->getTopic() + "\r\n";
        send(user.getFd().fd, topicResponse.c_str(), topicResponse.length(), 0);
    }

    // Envoi de la liste des utilisateurs
    std::string namesResponse = ":server 353 " + user.getNickName() + " = " + canalName + " :";
    std::set<User*> channelUsers = canal->getCurrentUsers();
    std::set<User*> chanOps = canal->getChanOps();

    for (std::set<User*>::iterator it = channelUsers.begin(); it != channelUsers.end(); ++it) {
        User *channelUser = *it;
        if (chanOps.find(channelUser) != chanOps.end()) {
            namesResponse += "@";
        }
        namesResponse += channelUser->getNickName() + " ";
    }
    namesResponse += "\r\n";
    send(user.getFd().fd, namesResponse.c_str(), namesResponse.length(), 0);

    // Envoi du message de fin de liste
    std::string endNamesResponse = ":server 366 " + user.getNickName() + " " + canalName + " :End of /NAMES list.\r\n";
    send(user.getFd().fd, endNamesResponse.c_str(), endNamesResponse.length(), 0);

    // Notification aux autres utilisateurs
    sendToChannelUsers(channelUsers, joinResponse, &user);

    std::cout << "User " << user.getNickName() << " joined canal " << canalName << std::endl;
}


// void CommandManager::handleJoin(std::string command, User &user) 
// {
//     Server &server = Server::getServer();

//     // Extraction du nom du canal
//     size_t spacePos = command.find(' ');
//     std::string canalName = command.substr(1, spacePos - 1);
//     if (canalName.empty() || canalName[0] != '#') {
//         std::cerr << "Invalid channel name" << std::endl;
//         return;
//     }
//     if (canalName.length() > 200) {
//         std::cerr << "Channel name too long" << std::endl;
//         return;
//     }

//     // Extraction du mot de passe
//     std::string password = extractPassword(command);

//     // Recherche ou création du canal
//     Canal *canal = server.getCanalByName(canalName);
//     bool isNewCanal = false;
//     if (canal == NULL) {
//         canal = new Canal(canalName);
//         server.addCanal(*canal);
//         isNewCanal = true;
//         std::cout << "Canal " << canalName << " created." << std::endl;
//     }

//     // Validation des restrictions du canal
//     if (!validateChannelRestrictions(*canal, user, password)) {
//         return;
//     }

//     // Ajout de l'utilisateur au canal
//     canal->addUser(user);
//     if (isNewCanal) {
//         canal->addChanOps(user); // Le premier utilisateur devient opérateur
//     }

//     // Envoi de la confirmation de JOIN
//     std::string joinResponse = ":" + user.getNickName() + "!~" + user.getUserName() + "@localhost JOIN " + canalName + "\r\n";
//     send(user.getFd().fd, joinResponse.c_str(), joinResponse.length(), 0);

//     // Envoi du topic du canal
//     if (!canal->getTopic().empty()) {
//         std::string topicResponse = ":server 332 " + user.getNickName() + " " + canalName + " :" + canal->getTopic() + "\r\n";
//         send(user.getFd().fd, topicResponse.c_str(), topicResponse.length(), 0);
//     }

//     // Envoi de la liste des utilisateurs
//     std::string namesResponse = ":server 353 " + user.getNickName() + " = " + canalName + " :";
//     std::set<User*> channelUsers = canal->getCurrentUsers();
//     std::set<User*> chanOps = canal->getChanOps();

//     for (std::set<User*>::iterator it = channelUsers.begin(); it != channelUsers.end(); ++it) {
//         User *channelUser = *it;
//         if (chanOps.find(channelUser) != chanOps.end()) {
//             namesResponse += "@"; // Préfixe pour les opérateurs
//         }
//         namesResponse += channelUser->getNickName() + " ";
//     }
//     namesResponse += "\r\n";
//     send(user.getFd().fd, namesResponse.c_str(), namesResponse.length(), 0);

//     // Envoi du message de fin de liste
//     std::string endNamesResponse = ":server 366 " + user.getNickName() + " " + canalName + " :End of /NAMES list.\r\n";
//     send(user.getFd().fd, endNamesResponse.c_str(), endNamesResponse.length(), 0);

//     // Notification aux autres utilisateurs
//     sendToChannelUsers(channelUsers, joinResponse, &user);

//     std::cout << "User " << user.getNickName() << " joined canal " << canalName << std::endl;
// }
