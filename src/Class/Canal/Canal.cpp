#include "Canal.hpp"

Canal::Canal(): fd(-1), user_limit(10) {}

Canal::Canal(int fd): fd(fd), user_limit(10) {}

Canal::~Canal() {}


Canal::Canal(Canal const &copy) : fd(copy.fd), user_limit(copy.user_limit), flags(copy.flags),
	password(copy.password), topic(copy.topic), cur_users(copy.cur_users),
	inv_users(copy.inv_users), chanops(copy.chanops) {}

Canal	&Canal::operator=(Canal const &rhs) {
	if (this != &rhs) {
		fd = rhs.fd;
		user_limit = rhs.user_limit;
		flags = rhs.flags;
		password = rhs.password;
		topic = rhs.topic;
		cur_users = rhs.cur_users;
		inv_users = rhs.inv_users;
		chanops = rhs.chanops;
	}
	return *this;
}