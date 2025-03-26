#include <cstring>
#include <cstdlib>
#include <signal.h>
#include "Class/Server/Server.hpp"
#include "Class/User/User.hpp"

int	checkPassingArguments(int argc, char **argv);
void handle_sigint(int signal);
void handle_sigquit(int sigquit);

int main(int argc, char **argv)
{
	if (!checkPassingArguments(argc, argv))
		return 1;

	// signal(SIGINT, &handle_sigint);
	Server &server = Server::init(std::atoi(argv[1]), argv[2]);
	signal(SIGINT, &handle_sigint);
	signal(SIGQUIT, &handle_sigquit);

	// try
	// {
		User *testt = new User(
			"salt",
			"salu",
			"ccpc"
		);
		server.addUser(*testt);
		Canal *testCanal = new Canal("coucou");
		testCanal->setHasProtectedTopic(false);
		server.addCanal(*testCanal);
		testCanal->addChanOps(*testt);
		testCanal->addUser(*testt);
		// std::cout << "result : " << UserManager::isUniqueUsername("salu");
		server.running();
	// }
	// catch(const std::exception& e)
	// {
	// 	std::cerr << e.what() << std::endl;
	// 	server.kill();
	// 	return 1;
	// }
	server.kill();
	return 0;
}