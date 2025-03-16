#ifndef USER_MANAGER_CLASS_HPP
#define USER_MANAGER_CLASS_HPP

#include "../../Class/User/User.hpp"
#include "../../Class/Server/Server.hpp"


class UserManager
{
	private:
		static	bool	isUniqueUsername(std::string const &username);
		static	bool    isUniqueNickName(std::string const &nickName);
	public:
		static	void	createNewUser(std::string clientMsg, int fd);
};



#endif