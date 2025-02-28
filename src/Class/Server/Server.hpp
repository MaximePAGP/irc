#ifndef SERVER_CLASS_HPP
#define SERVER_CLASS_HPP

#include "../Canal/Canal.hpp"
#include "../User/User.hpp"
#include <string>
#include <set>

void	printUsers(std::set<User>);

class Server
{
	private:
		Server();
		std::string 			password;
		std::string	const 		portname;
		std::set<User>			serverOps;
		std::set<Canal>			canals;
	public:
		Server(std::string portname, std::string password);
		virtual	~Server();
		Server(Server const &copy);
		Server &operator=(Server const &rhs);

		std::string 			getPassord() const;
		std::string				getPortname() const;
		std::set<User>			getServerOps() const;
		std::set<Canal>			getCanals() const;

		void					setPassword(std::string newPassword);

		std::pair<std::set<User>::iterator, bool>
								addServerOps(User newServOP);
		std::size_t				removeServerOps(User target);
		
		std::pair<std::set<Canal>::iterator, bool>
								addCanal(Canal newCanal);
		std::size_t				removeCanal(Canal target);
};

std::ostream &operator<<(std::ostream &out, Server const &rhs);

#endif