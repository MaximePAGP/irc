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

// JOIN #1,#2,#3 mdp1,,

void CommandManager::handleJoin(std::string command, User &user) 
{
    Server &server = Server::getServer();
    // Remove trailing whitespace or newlines
    std::string canalName = command;
    canalName = canalName.substr(1, canalName.size());
    std::cout << "canalName : " << canalName << std::endl;
    if (canalName.empty())
    {
        std::cerr << "Channel name is empty" << std::endl;
        return;
    }
    if (canalName[0] != '#')
    {
        std::cerr << "Invalid channel name #" << std::endl;
        return;
    }

    // AJOUTER ICI: Extraction du mot de passe s'il existe
    std::string password = "";
    size_t spacePos = command.find(' ');
    if (spacePos != std::string::npos) 
    {
        password = command.substr(spacePos + 1);
        // Supprimer espaces et caractères de fin de ligne
        size_t pwEndPos = password.find_first_of(" \r\n");
    if (pwEndPos != std::string::npos)
    {
        password = password.substr(0, pwEndPos);
    }
    }
    // Find or create the channel - using your getCanalByName function
    Canal* canal = server.getCanalByName(canalName);
    if (canal == NULL) 
    {
        // Create new channel
        canal = new Canal(canalName);
        server.addCanal(*canal);
        std::cout << "Canal " << canalName << " created." << std::endl;
    }
    // AJOUTER ICI: Vérification du mot de passe si canal existe
    if (canal != NULL && canal->isProtectedByPassword() && password != canal->getPassword()) 
    {
        std::string errorMsg = ":server 475 " + user.getNickName() + " " + canalName + " :Cannot join channel (+k) - bad key\r\n";
        send(user.getFd().fd, errorMsg.c_str(), errorMsg.length(), 0);
        return;
    }
    canal->addUser(user);
    canal->addChanOps(user);

    // Send JOIN confirmation to the user - fixed format
    std::string joinResponse = ":" + user.getNickName() + "!~" + user.getUserName() + "@localhost JOIN " + canalName + "\r\n";
    // std::string joinResponse = ":server 329 reo1 JOIN #general\r\n";
    std::cout << "Join response: " << joinResponse << std::endl;
    send(user.getFd().fd, joinResponse.c_str(), joinResponse.length(), 0);
    std::cout << "Join response sent to user " << user.getNickName() << std::endl;
    // Send channel topic if it exists
    std::string topic = canal->getTopic();
    if (!topic.empty()) {
        std::string topicResponse = ":server 332 " + user.getNickName() + " " + canalName + " :" + topic + "\r\n";
        send(user.getFd().fd, topicResponse.c_str(), topicResponse.length(), 0);
    }
    // Send names list (users in channel)
    std::string namesResponse = ":server 353 " + user.getNickName() + " = " + canalName + " :";

    std::set<User*> channelUsers = canal->getCurrentUsers();
    std::set<User*> channelOps = canal->getChanOps();
    std::set<User*>::iterator userIt;

    for (userIt = channelUsers.begin(); userIt != channelUsers.end(); ++userIt) {
        User* channelUser = *userIt;

        // Ajouter le préfixe @ pour les chanops
        if (channelOps.find(channelUser) != channelOps.end()) {
            namesResponse += "@";
        }

        namesResponse += channelUser->getNickName() + " ";
    }

    namesResponse += "\r\n";

    // Envoyer la liste des membres à tous les utilisateurs du canal
    for (userIt = channelUsers.begin(); userIt != channelUsers.end(); ++userIt) {
        send((*userIt)->getFd().fd, namesResponse.c_str(), namesResponse.length(), 0);
    }

    // Envoyer le message de fin de liste
    std::string endNamesResponse = ":server 366 " + user.getNickName() + " " + canalName + " :End of /NAMES list.\r\n";
    for (userIt = channelUsers.begin(); userIt != channelUsers.end(); ++userIt) {
        send((*userIt)->getFd().fd, endNamesResponse.c_str(), endNamesResponse.length(), 0);
    }
    
    // Notify other users in the channel
    for (userIt = channelUsers.begin(); userIt != channelUsers.end(); ++userIt) {
        User* channelUser = *userIt;
        
        // Skip the joining user (already notified)
        if (channelUser == &user) {
            continue;
        }
        
        int userFd = channelUser->getFd().fd;
        send(userFd, joinResponse.c_str(), joinResponse.length(), 0);
    }
    
    std::cout << "User " << user.getNickName() << " joined canal " << canalName << std::endl;
}
