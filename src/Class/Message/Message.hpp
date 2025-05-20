#ifndef MESSAGE_CLASS_HPP
#define MESSAGE_CLASS_HPP

#include <sys/socket.h>
#include <string>
#include "../User/User.hpp"
#include "../Channel/Channel.hpp"

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


		static	void	userToLong(User const &user, std::string const nickname);
		static	void	userAlreadyTaken(User const &user, std::string const nickname);
		static	void	userForbiddenChar(User const &user, std::string const nickname);
		static	void	userCannotChange(User const &user, std::string const nickname);
		static	void	userSet(User const &user);

		static	void	modeNotSuchChannel(User const &user, std::string const channelName);

		static	void	topicSetTopic(User const &user, Channel const &channel);
		static	void	topicNoTopic(User const &user, std::string const channelName);
		static	void	topicGetTopic(User const &user, Channel const &channel);

		static	void	noSuchCommand(User const &user, std::string const &command);
		static	void	commandToLong(User const &user);

		static	void	alreadyOnChannel(User const &user, Channel const &channel);


		static	void	notEnoughParams(User const &user, std::string const &command);

		static	void	kickSucces(User const &user, Channel const &chanel, std::string const &target, std::string const reason);

		static	void	partYoureNotInChan(User const &user, std::string const &chanName);
		static	void	partNotification(User const &user, std::string const &chanName, std::string const &userLeft, std::string const reason);
	};





#endif
