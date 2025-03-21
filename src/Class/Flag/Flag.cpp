#include "Flag.hpp"

Flag::Flag() {}

Flag::~Flag() {}

Flag::Flag(Flag const &copy): flags(copy.flags) {}

Flag &Flag::operator=(Flag const &rhs) {
	if (&rhs != this) {
    	this->flags = rhs.flags;
	}
    return *this;
}

std::pair<std::set<int>::iterator, bool>	Flag::addFlag(int flag) {
	return this->flags.insert(flag);
}


// work exactly like bool
std::size_t	Flag::removeFlag(int flag) {
	return this->flags.erase(flag);
}


bool	Flag::hasFlag(ChanFlag flag) {
	return this->flags.find(flag) != this->flags.end();
}

bool	Flag::hasFlag(UserFlag flag) {
	return this->flags.find(flag) != this->flags.end();
}