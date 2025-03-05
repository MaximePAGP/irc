#include "../Class/Server/Server.hpp"


void handle_sigint(int sigint) {
	// Server &server = Server::getServer();
	(void)sigint;
	// server.setState(false);
	// server.kill();
}
