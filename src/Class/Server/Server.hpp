#ifndef SERVER_CLASS_HPP
#define SERVER_CLASS_HPP

#include <set>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <stdbool.h>
#include "../Canal/Canal.hpp"
#include "../User/User.hpp"
#include "ServerExepction.hpp"


#define	MSG_LEN 512

void	printUsers(std::set<User>);

class Server: public ServerExepction
{
	private:
		Server();
		Server(int portname, std::string password);
		Server(Server const &copy);
		Server &operator=(Server const &rhs);
		
		std::string 			password;
		int 					portname;
		int						socketFd;
		bool					isRunning;
		std::set<User>			serverOps;
		std::set<Canal>			canals;
		void					createSocket(); // By default we use TCP and IPV4
		void					bindAndListenPort();
		void					handleClient(int clientFd);
	public:
		virtual	~Server();
		static	Server			&init(int portname, std::string password);
		static	Server			&getServer();
		std::string 			getPassord() const;
		int						getPortname() const;
		bool					getState() const;
		std::set<User>			getServerOps() const;
		std::set<Canal>			getCanals() const;

		void					setPassword(std::string newPassword);
		void					setState(bool value);

		std::pair<std::set<User>::iterator, bool>
								addServerOps(User newServOP);
		std::size_t				removeServerOps(User target);
		
		std::pair<std::set<Canal>::iterator, bool>
								addCanal(Canal newCanal);
		std::size_t				removeCanal(Canal target);

		void					running();
		void					kill();
};

std::ostream &operator<<(std::ostream &out, Server const &rhs);

#endif