

#ifndef COMMAND_MANAGER_CLASS_HPP
#define COMMAND_MANAGER_CLASS_HPP

#include <algorithm>
#include <string>
#include <iostream>
#include "../../Class/Server/Server.hpp"


#define	MSG_LEN 512 // Following norm IRC 1459, message size limit is <-
#define LIMIT_USERNAME_NICKNAME 9

class CommandManager
{
	private:
		static	void	handleNick(std::string command, User &user);
		static	void	handleUsername(std::string command, User &user);
		static	void	handleMode(std::string command, User &user);
		static	void	handleJoin(std::string command, User &user);
		static	void	handleTopic(std::string command, User &user);
		static	bool	hasValidCommand(std::string command);
		static	bool	commandIsComplete(std::string command);
		static	std::string	trimFirstParamSpace(std::string param);
		CommandManager();
    public:
		virtual  ~CommandManager();
		static	void	redirectCommand(std::string command, User &user);
		static 	void	buildCommand(std::string command, int clientFd);
};


#endif