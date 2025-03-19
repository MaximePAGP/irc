

#ifndef COMMAND_MANAGER_CLASS_HPP
#define COMMAND_MANAGER_CLASS_HPP

#include <algorithm>
#include <string>
#include <iostream>
#include "../../Class/Server/Server.hpp"


#define	MSG_LEN 512 // Following norm IRC 1459, message size limit is <-

class CommandManager
{
	private:
		static	bool 	hasForbiddenChar(std::string nickname);
		static	void	handleNick(std::string command, User &user);
		static	bool	hasValidCommand(std::string command);
		static	bool	commandIsComplete(std::string command);
		static	std::string	trimParamSpace(std::string param);
		CommandManager();
    public:
		virtual  ~CommandManager();
		static	void	redirectCommand(std::string command, User &user);
		static void		buildCommand(std::string command, int clientFd);
};


#endif