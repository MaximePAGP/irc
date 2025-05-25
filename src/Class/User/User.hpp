#ifndef USER_CLASS_HPP
#define USER_CLASS_HPP

#include <iostream>
#include <cstring>
#include <map>
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
		bool			isConnected;
		std::set<std::string> channelsName;
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
		bool			getIsConnected() const;
		std::set<std::string> getChannelsName() const;

		void			setNickName(std::string newNickname);
		void			setUsername(std::string newUsername);
		void			setPassword(std::string newPassord);
		void			setFd(struct pollfd newFd);
		void			setCommandBuffer(std::string value);
		void			setIsConnected(bool status);

		void			flushCommandBuffer();
		static	bool	hasForbiddenNickChar(std::string nickname);
		static	bool	hasForbiddenUsernameChar(std::string usnername);

		void			addChannelName(std::string &name);
		void			removeChannelName(std::string const &name);

		static	std::string	replaceSpecialChar(std::string param);

		bool			operator<(const User &other)const;
};

void	printUsers(std::set<User*>);
std::ostream &operator<<(std::ostream &out, User const &rhs);


#endif
