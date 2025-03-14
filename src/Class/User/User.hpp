#ifndef USER_CLASS_HPP
#define USER_CLASS_HPP

#include <iostream>
#include <string>
#include <set>
#include <poll.h>


class User
{
	private:
		struct	pollfd	fd;
		std::string 	nickName; // Required and can be changed
		std::string 	const userName; // Required and unique and cannot be changed
		std::string 	password; // Optionnal maybe usless
		User();
	public:
		User(std::string const &nickName, std::string const &userName, std::string password);
		virtual ~User();
		User (User const &cpy);
		User &operator=(User const &rhs);

		std::string		getNickName() const;
		std::string		getUserName() const;
		std::string		getPassword() const;
		struct	pollfd	getFd() const;
		void			setNickName(std::string newNickname);
		void			setPassword(std::string newPassord);
		void			setFd(struct pollfd newFd);
	
		bool			operator<(const User &other)const;
};

void	printUsers(std::set<User>);
std::ostream &operator<<(std::ostream &out, User const &rhs);


#endif