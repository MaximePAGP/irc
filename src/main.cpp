#include <cstring>
#include <cstdlib>
#include <signal.h>
#include "Class/Server/Server.hpp"


int	checkPassingArguments(int argc, char **argv);
void handle_sigint(int sigint);

int main(int argc, char **argv)
{
	if (!checkPassingArguments(argc, argv))
		return 1;

	signal(SIGINT, &handle_sigint);

	Server &server = Server::init(std::atoi(argv[1]), argv[2]);

	server.running();
	server.kill();
	return 0;
}