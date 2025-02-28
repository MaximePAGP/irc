
#ifndef FLAG_CLASS_HPP
#define  FLAG_CLASS_HPP

#include <iostream>
#include <string>
#include <set>

enum UserFlag {
    ServerOp,
    ChanOp
};

enum ChanFlag {
    Private,
    InvitationOnly,
    PasswordProtected
};


class Flag
{
	private:
		std::set<int> flags;
	public:
		Flag();
		Flag(Flag const &copy);
		Flag &operator=(Flag const &rhs);
		virtual ~Flag();

		std::pair<std::set<int>::iterator, bool> addFlag(int flag);
		std::size_t removeFlag(int flag);
		bool		hasFlag(ChanFlag flag);
		bool		hasFlag(UserFlag flag);

};	

#endif