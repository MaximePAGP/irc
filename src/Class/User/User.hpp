#ifndef USER_CLASS_HPP
#define USER_CLASS_HPP

#include <iostream>
#include <string>
#include <set>

class User
{
	private:
		int			fd;
		std::string nickName; // Required and can be changed
		std::string const userName; // Required and unique and cannot be changed
		std::string password; // Optionnal maybe usless
		User();
	public:
		User(std::string &nickName, std::string const &userName, std::string password, int fd);
		virtual ~User();
		User (User const &cpy);
		User &operator=(User const &rhs);

		std::string		getNickName() const;
		std::string		getUserName() const;
		std::string		getPassword() const;
		int				getFd() const;
		void			setNickName(std::string newNickname);
		void			setPassword(std::string newPassord);
		void			setFd(int newFd);
	
		bool			operator<(const User &other)const;
};

void	printUsers(std::set<User>);
std::ostream &operator<<(std::ostream &out, User const &rhs);


#endif