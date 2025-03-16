

#ifndef COMMAND_MANAGER_CLASS_HPP
#define COMMAND_MANAGER_CLASS_HPP

#include <algorithm>
#include <string>
#include <iostream>

class CommandManager
{
    private:
		bool	isNick(std::string command);
        bool	hasValidCommand(std::string command);
        CommandManager();
    public:
        virtual  ~CommandManager();
		bool	commandIsComplete(std::string command);
		void	redirectCommand(std::string command);
};


#endif