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
		std::string 	userName; // Required and unique and cannot be changed
		std::string 	password; // Optionnal maybe usless
		std::string		commandBuffer; // used to fill the command until we found \r\n (meaning its the end of command)
		User();
	public:
		User(std::string nickName, std::string userName, std::string password);
		virtual ~User();
		User (User const &cpy);
		User &operator=(User const &rhs);

		std::string		getNickName() const;
		std::string		getUserName() const;
		std::string		getPassword() const;
		struct	pollfd	getFd() const;
		std::string		getCommandBuffer() const;
		void			setNickName(std::string newNickname);
		void			setUsername(std::string newUsername);
		void			setPassword(std::string newPassord);
		void			setFd(struct pollfd newFd);
		void			setCommandBuffer(std::string value);
		void			flushCommandBuffer();

		bool			operator<(const User &other)const;
};

void	printUsers(std::set<User*>);
std::ostream &operator<<(std::ostream &out, User const &rhs);


#endif