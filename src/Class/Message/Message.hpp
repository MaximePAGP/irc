#ifndef MESSAGE_CLASS_HPP
#define MESSAGE_CLASS_HPP

#include <sys/socket.h>
#include <string>
#include "../User/User.hpp"

#define ENV 	"localhost"
#define END_CMD	"\r\n"

class Message
{
	public:
		static	void	youreNotChanOp(std::string chanName, User &user);
		static	void	noSuchNickChannel(std::string target, User &user);
		static	void	changedModeChan(std::string chanName, User &user, std::string flag);
		static	void	unknowFlag(User &user, std::string flag);
		static	void	chanPasswordForbiddenChar(std::string chanName, User &user, std::string password);
		static	void	chanPassordToLong(std::string chanName, User &user, std::string password);

		static	void	nickNoParam(User &user, std::string nickname);
		static	void	nickToLongParam(User &user, std::string nickname);
		static	void	nickAlreadyUsed(User &user, std::string nickname);
		static	void	nickIllegal(User &user, std::string nickname);
		static	void	nickSet(User &user);
		static	void	nickSetUpdated(User &user, std::string oldNick);


		static	void	userNoParam(User &user, std::string nickname);
		static	void	userToLong(User &user, std::string nickname);
		static	void	userAlreadyTaken(User &user, std::string nickname);
		static	void	userForbiddenChar(User &user, std::string nickname);
		static	void	userCannotChange(User &user, std::string nickname);
		static	void	userSet(User &user);

};





#endif