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
    // Find or create the channel - using your getCanalByName function
    Canal* canal = server.getCanalByName(canalName);
    if (canal == NULL) 
    {
        // Create new channel
        canal = new Canal(canalName);
        server.addCanal(*canal);
        std::cout << "Canal " << canalName << " created." << std::endl;
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
        
        // Check if this user is a channel operator
        std::set<User*>::iterator opIt = channelOps.find(channelUser);
        if (opIt != channelOps.end()) 
        {
            namesResponse += "@";
        }
        
        namesResponse += channelUser->getNickName() + " ";
    }
    
    namesResponse += "\r\n";
    send(user.getFd().fd, namesResponse.c_str(), namesResponse.length(), 0);
    
    // End of names list
    std::string endNamesResponse = ":server 366 " + user.getNickName() + " " + canalName + " :End of /NAMES list.\r\n";
    send(user.getFd().fd, endNamesResponse.c_str(), endNamesResponse.length(), 0);
    
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
