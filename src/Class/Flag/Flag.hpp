
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

		bool addFlag(int flag);
		bool removeFlag(int flag);
};	

#endif