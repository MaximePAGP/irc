#include "Flag.hpp"

Flag::Flag() {}

Flag::~Flag() {}

Flag::Flag(Flag const &copy): flags(copy.flags) {}

Flag &Flag::operator=(Flag const &rhs) {
	if (&rhs == this)
		return *this;
    this->flags = rhs.flags;
    return *this;
}

bool	Flag::addFlag(int flag) {
	return this->flags.insert(flag);
}

bool	Flag::removeFlag(int flag) {
	return this->flags.erase(flag);
}
