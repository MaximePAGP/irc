#ifndef MESSAGE_CLASS_HPP
#define MESSAGE_CLASS_HPP

#include <sys/socket.h>
#include <string>
#include "../User/User.hpp"
#include "../Canal/Canal.hpp"

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

		static	void	modeNotEnoughParams(User &user);
		static	void	modeNotSuchChannel(User &user, std::string canalName);

		static	void	topicSetTopic(User &user, std::string canalName, std::string topic);
		static	void	topicNoTopic(User const &user, std::string const canalName);
		static	void	topicGetTopic(User const &user, Canal const &canal);
	};





#endif