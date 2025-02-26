#ifndef CANAL_CLASS_HPP
#define CANAL_CLASS_HPP


#include <string>
#include <set>
#include "../Flag/Flag.hpp"
#include "../User/User.hpp"

class Canal
{
	private:
		Canal();

		int						fd;
		int						user_limit;
		Flag					flags;
		std::string				password;
		std::string				topic;
		std::set<User>			cur_users;
		std::set<User>			inv_users;
		std::set<User>			chanops;
	public:
		Canal(int fd);
		virtual ~Canal();
		Canal(Canal const &copy);
		Canal &operator=(Canal const &rhs);
};


#endif