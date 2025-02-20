#ifndef SERVER_CLASS_HPP
#define SERVER_CLASS_HPP

#include <stdbool.h>
#include <string>

class Server
{
	private:
		std::string password;
		std::string portname;
		Server();
	public:

		~Server();
};


#endif