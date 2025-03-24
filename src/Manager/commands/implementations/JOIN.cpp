#include "../CommandManager.hpp"


void CommandManager::handleJoin(std::string command, User &user) {
    (void)command;
    (void)user;
    // Remove trailing whitespace or newlines
    // size_t endPosition = canalName.find_first_of(" \r\n");
    // if (endPosition != std::string::npos)
    // {
    //     canalName = canalName.substr(0, endPosition);
    // }
    
    // if (canalName.empty())
    // {
    //     std::cerr << "Channel name is empty" << std::endl;
    //     return;
    // }
    
    // // Validate channel name
    // if (canalName[0] != '#') 
    // {
    //     canalName = "#" + canalName;
    // }
    
    // // Find or create the channel - using your getCanalByName function
    // Canal* canal = this->getCanalByName(canalName);
    // if (canal == NULL) 
    // {
    //     // Create new channel
    //     canal = new Canal(canalName);
    //     this->addCanal(*canal);
    //     std::cout << "Canal " << canalName << " created." << std::endl;
    // }
    
    // // Add user to the channel
    // canal->addUser(*joiningUser);
    
    // // Make joining user a channel operator
    // canal->addChanOps(*joiningUser);
    
    // // Send JOIN confirmation to the user - fixed format
    // std::string joinResponse = ":" + joiningUser->getNickName() + "!~" + joiningUser->getUserName() + "@localhost JOIN " + canalName + "\r\n";
    // send(user.getFd().fd, joinResponse.c_str(), joinResponse.length(), 0);
    
    // // Send channel topic if it exists
    // std::string topic = canal->getTopic();
    // if (!topic.empty()) {
    //     std::string topicResponse = ":server 332 " + joiningUser->getNickName() + " " + canalName + " :" + topic + "\r\n";
    //     send(user.getFd().fd, topicResponse.c_str(), topicResponse.length(), 0);
    // }
    
    // // Send names list (users in channel)
    // std::string namesResponse = ":server 353 " + joiningUser->getNickName() + " = " + canalName + " :";
    
    // std::set<User*> channelUsers = canal->getCurrentUsers();
    // std::set<User*> channelOps = canal->getChanOps();
    // std::set<User*>::iterator userIt;
    
    // for (userIt = channelUsers.begin(); userIt != channelUsers.end(); ++userIt) {
    //     User* channelUser = *userIt;
        
    //     // Check if this user is a channel operator
    //     std::set<User*>::iterator opIt = channelOps.find(channelUser);
    //     if (opIt != channelOps.end()) 
    //     {
    //         namesResponse += "@";
    //     }
        
    //     namesResponse += channelUser->getNickName() + " ";
    // }
    
    // namesResponse += "\r\n";
    // send(user.getFd().fd, namesResponse.c_str(), namesResponse.length(), 0);
    
    // // End of names list
    // std::string endNamesResponse = ":server 366 " + joiningUser->getNickName() + " " + canalName + " :End of /NAMES list.\r\n";
    // send(user.getFd().fd, endNamesResponse.c_str(), endNamesResponse.length(), 0);
    
    // // Notify other users in the channel
    // for (userIt = channelUsers.begin(); userIt != channelUsers.end(); ++userIt) {
    //     User* channelUser = *userIt;
        
    //     // Skip the joining user (already notified)
    //     if (channelUser == joiningUser) {
    //         continue;
    //     }
        
    //     int userFd = channelUser->getFd().fd;
    //     send(userFd, joinResponse.c_str(), joinResponse.length(), 0);
    // }
    
    // std::cout << "User " << joiningUser->getNickName() << " joined canal " << canalName << std::endl;
}
