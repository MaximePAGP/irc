#include "../Class/Server/Server.hpp"


void handle_sigint(int sigint) {
	Server &server = Server::getServer();
	(void)sigint;
	server.setState(false);
}

void handle_sigquit(int sigquit) {
	Server &server = Server::getServer();
	(void)sigquit;
	std::cout << "Quit (core dumped)" << std::endl;
	server.setState(false);
}