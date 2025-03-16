

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
		bool	isNick(std::string command);
        bool	hasValidCommand(std::string command);
		bool	commandIsComplete(std::string command);
        CommandManager();
    public:
        virtual  ~CommandManager();
		void	redirectCommand(std::string command);
		static void	buildCommand(std::string command, int clientFd);
};


#endif