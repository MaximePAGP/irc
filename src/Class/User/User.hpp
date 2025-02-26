#ifndef USER_CLASS_HPP
#define USER_CLASS_HPP

#include <iostream>
#include <string>

class User
{
	private:
		std::string nickName; // Required and can be changed
		std::string const userName; // Required and unique and cannot be changed
		std::string password; // Optionnal maybe usless
		User();
	public:
		User(std::string &nickName, std::string const &userName, std::string password);
		virtual ~User();
		User (User const &cpy);
		User &operator=(User const &rhs);
};


#endif