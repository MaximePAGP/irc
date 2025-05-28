#include "Server.hpp"


void	Server::running() {
	this->initServerSocket();
	this->bindAndListenPort();
	this->setState(true);
	std::cout << "Server is running on port : " << this->getPortname() << std::endl;
	while (this->getState()) {
		int	pollEvent = poll(this->sockets.data(), this->sockets.size(), -1);
		if (pollEvent == -1)
			break;

		for (size_t i = 0; i < this->sockets.size(); i++) {
			if (this->sockets[i].revents & POLLIN) {
				std::cout << "trigger " << std::endl;
				if (i == 0)
					this->createNewClient();
				else
					this->handleClientMsg(this->sockets[i].fd);
			}
		}
	}
}


void	Server::bindAndListenPort() {
	struct sockaddr_in	address;
	int				option = 1;	

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(this->getPortname());
	if (setsockopt(this->sockets[0].fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) == -1)
        throw ServerExepction::CannotSetSocketOptionException();
	if (bind(this->sockets[0].fd, (struct sockaddr*) &address, sizeof(address)) == -1)
		throw ServerExepction::CannotBindPortException();
	if (listen(this->sockets[0].fd, 10) < 0)
		throw ServerExepction::CannotListenException();
	if (fcntl(this->sockets[0].fd, F_SETFL, O_NONBLOCK) == -1)
		throw ServerExepction::CannotSetFdOptionsException();
}

void	Server::initServerSocket() {
	struct pollfd serverPollFd;

	serverPollFd.fd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverPollFd.fd < 0)
		throw SocketCreationException();
	serverPollFd.events = POLLIN;
	this->sockets.push_back(serverPollFd);	
}

// handle control D entre les buffers et control Z pour les messages recu pendant que le processus est en background

void	Server::handleClientMsg(int clientFd) {
	char buffer[MSG_LEN];
	ssize_t bytesRead = 1;

	while ((bytesRead = recv(clientFd, buffer, sizeof(buffer), 0)) > 0) {
		buffer[bytesRead] = '\0';
		CommandManager::buildCommand(buffer, clientFd);
		std::cout << "Received : <" << buffer << ">" << std::endl;
		// send(clientFd, "Message received", 16, 0);
	}
	// handle if recv crash send a response to client to say we cannot handle the commmand
	if (bytesRead == 0) {
		this->handleClientLogout(clientFd);
	}
}

void	Server::handleClientLogout(int clientFd) {

	for (std::vector<struct pollfd>::iterator it = this->sockets.begin(); it != this->sockets.end(); it++)
	{
		pollfd cur = *it;
		if (cur.fd == clientFd) {
			this->sockets.erase(it);
			break ;
		}
	}

	User* leavingUser = this->getUserByFd(clientFd);

	std::set<Channel *> allChannels = this->getChannels();
	std::set<Channel *>::iterator itChannels = allChannels.begin();

	while (itChannels != allChannels.end()) {
		(*itChannels)->removeUserInvitation(*leavingUser);
		itChannels++;
	}

	std::set<std::string> userChannels =  leavingUser->getChannelsName();
	std::set<std::string>::iterator chanIt = userChannels.begin();
	
	while (chanIt != userChannels.end()) {
		Channel *channel = this->getChannelByName(*chanIt);
		if (!channel)
			continue;

		channel->removeChanOps(*leavingUser);	
		channel->removeUserInvitation(*leavingUser);	
		channel->removeUser(*leavingUser);	
		
		std::set<User *> userInChan = channel->getCurrentUsers();	
		std::set<User *>::iterator itUserToNotify = userInChan.begin();

		while (itUserToNotify != userInChan.end()) {
			Message::partNotification(*(*itUserToNotify), channel->getName(), leavingUser->getNickName());
			itUserToNotify++;
		}

		chanIt++;
	}


	this->users.erase(leavingUser);  
	delete leavingUser;


	Server::pclose(clientFd);
	std::cout << "Client disconected" << std::endl;
}

void Server::createNewClient() {
    struct sockaddr_in	clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    int clientFd = accept(this->sockets[0].fd, (struct sockaddr*)&clientAddr, &addrLen);
    if (clientFd < 0) {
		std::cerr << "New connexion was failed" << std::endl;
		return ;
	}
	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1) {
		std::cerr << "Fcntl crash and close connection !" << std::endl;
		Server::pclose(clientFd);
		return ;
	}
	std::cout << "New client connected : " << clientFd << std::endl;

	struct pollfd newClient;

	newClient.fd = clientFd;
	newClient.events = POLLIN;
	newClient.revents = 0;
	this->sockets.push_back(newClient);
	User *user = new User("", "", "");
	user->setFd(newClient);
	this->addUser(*user);
}
