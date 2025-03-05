#include <cstring>
#include <cstdlib>

#include "Class/Server/Server.hpp"


int	checkPassingArguments(int argc, char **argv);

int main(int argc, char **argv)
{
	if (!checkPassingArguments(argc, argv))
		return 1;

	Server &server = Server::init(std::atoi(argv[1]), argv[2]);

	server.running();
	server.kill();
	return 0;
}