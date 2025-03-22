#include "Canal.hpp"

Canal::Canal(): userLimit(10), name("DEFAULT") {}

Canal::Canal(std::string name): userLimit(10), name(name), password(""), topic("") {}

Canal::~Canal() {}


Canal::Canal(Canal const &copy) : userLimit(copy.userLimit), flags(copy.flags),
	password(copy.password), topic(copy.topic), curUsers(copy.curUsers),
	invUsers(copy.invUsers), chanOp(copy.chanOp) {}

Canal	&Canal::operator=(Canal const &rhs) {
	if (this != &rhs) {
		userLimit = rhs.userLimit;
		flags = rhs.flags;
		password = rhs.password;
		topic = rhs.topic;
		curUsers = rhs.curUsers;
		invUsers = rhs.invUsers;
		chanOp = rhs.chanOp;
	}
	return *this;
}

int Canal::getUserLimits() const {
	return this->userLimit;
}

Flag	Canal::getFlag() const {
	return this->flags;
}

std::string	Canal::getName() const {
	return this->name;
}

std::string	Canal::getPassword() const {
	return this->password;
}

std::string	Canal::getTopic() const {
	return this->topic;
}

std::set<User*> Canal::getCurrentUsers() const {
	return this->curUsers;
}

std::set<User*> Canal::getUserInvitation() const {
	return this->invUsers;
}

std::set<User*> Canal::getChanOps() const {
	return this->chanOp;
}



void	Canal::setUserlimit(int value) {
	this->userLimit = value;
}

void	Canal::setPassword(std::string value) {
	this->password = value;
}

void	Canal::setTopic(std::string value) {
	this->topic = value;
}

std::pair<std::set<User*>::iterator, bool> Canal::addChanOps(User &value) {
	return this->chanOp.insert(&value);
}

std::pair<std::set<User*>::iterator, bool> Canal::addUser(User &value) {
	return this->curUsers.insert(&value);
}

std::pair<std::set<User*>::iterator, bool> Canal::addUserInvitation(User &value) {
	return this->invUsers.insert(&value);
}


std::string	Canal::replaceSpecialChar(std::string name) {
	std::string tmp = name;

	for (size_t i = 0; i < tmp.size(); i++)
	{
		if (tmp[i] == '{' || tmp[i] == '[')
			tmp[i] = ' ';
		else if (tmp[i] == '}' || tmp[i] == ']')
			tmp[i] = '\v';
	}
	
	return tmp;	
}


std::size_t	Canal::removeChanOps(User &target) {
	return this->chanOp.erase(&target);
}

std::size_t	Canal::removeUser(User &target) {
	return this->curUsers.erase(&target);
}

std::size_t	Canal::removeUserInvitation(User &target) {
	return this->invUsers.erase(&target);
}

bool	Canal::operator<(const Canal &other) const {
	return this->getName() < other.getName();
}


User	*Canal::getChanOpByFd(int fd) const {
	for (std::set<User*>::const_iterator it = this->chanOp.begin(); it != this->chanOp.end(); it++) {
		if ((*it)->getFd().fd == fd) {
			return *it;
		}
	}

	return NULL;
}

User	*Canal::getChanOpByNickname(std::string nickname) const {
	for (std::set<User*>::const_iterator it = this->chanOp.begin(); it != this->chanOp.end(); it++) {
		std::string tmpNick = User::replaceSpecialChar((*it)->getNickName());
		if (tmpNick == nickname) {
			return *it;
		}
	}

	return NULL;
}

User	*Canal::getChanOpByUsername(std::string username) const {
	for (std::set<User*>::const_iterator it = this->chanOp.begin(); it != this->chanOp.end(); it++) {
		std::string tmpUsername = User::replaceSpecialChar((*it)->getNickName());
		if (tmpUsername == username) {
			return *it;
		}
	}

	return NULL;
}


std::ostream &operator<<(std::ostream &out, Canal const &rhs) {
	out << rhs.getPassword() << " " << rhs.getTopic() << " "
		<< rhs.getUserLimits() << " ";
	out << "ChanOps : ";
	printUsers(rhs.getChanOps());
	out << "Current Users : ";
	printUsers(rhs.getCurrentUsers());
	out << " User invitations : ";
	printUsers(rhs.getUserInvitation());
	return out;
}

void			printCanals(std::set<Canal*> canals) {
	for (std::set<Canal*>::iterator it = canals.begin(); it != canals.end(); ++it)
    	std::cout << *it;
}
