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
		static	void	youreNotChanOp(std::string const chanName, User const &user);
		static	void	noSuchNickChannel(std::string const target, User const &user);
		static	void	changedModeChan(std::string const chanName, User const &user, std::string const flag);
		static	void	unknowFlag(User const &user, std::string const flag);
		static	void	chanPasswordForbiddenChar(std::string const chanName, User const &user, std::string const password);
		static	void	chanPassordToLong(std::string const chanName, User const &user, std::string const password);

		static	void	nickNoParam(User const &user, std::string const nickname);
		static	void	nickToLongParam(User const &user, std::string const nickname);
		static	void	nickAlreadyUsed(User const &user, std::string const nickname);
		static	void	nickIllegal(User const &user, std::string const nickname);
		static	void	nickSet(User const &user);
		static	void	nickSetUpdated(User const &user, std::string const oldNick);


		static	void	userNoParam(User const &user, std::string const nickname);
		static	void	userToLong(User const &user, std::string const nickname);
		static	void	userAlreadyTaken(User const &user, std::string const nickname);
		static	void	userForbiddenChar(User const &user, std::string const nickname);
		static	void	userCannotChange(User const &user, std::string const nickname);
		static	void	userSet(User const &user);

		static	void	modeNotEnoughParams(User const &user);
		static	void	modeNotSuchChannel(User const &user, std::string const canalName);

		static	void	topicSetTopic(User const &user, Canal const &canal);
		static	void	topicNoTopic(User const &user, std::string const canalName);
		static	void	topicGetTopic(User const &user, Canal const &canal);
	};





#endif