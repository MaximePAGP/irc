#include "Channel.hpp"

Channel::Channel(): userLimit(10), name("DEFAULT") {}

Channel::Channel(std::string name): userLimit(10), invitationOnly(false), hasProtectedTopic(true), name(name), password(""), topic("") {}

Channel::~Channel() {}


Channel::Channel(Channel const &copy) : userLimit(copy.userLimit),
	invitationOnly(copy.invitationOnly), hasProtectedTopic(copy.hasProtectedTopic),
	password(copy.password), topic(copy.topic), curUsers(copy.curUsers),
	invUsers(copy.invUsers), chanOp(copy.chanOp) {}

Channel	&Channel::operator=(Channel const &rhs) {
	if (this != &rhs) {
		userLimit = rhs.userLimit;
		password = rhs.password;
		topic = rhs.topic;
		invitationOnly = rhs.invitationOnly;
		hasProtectedTopic = rhs.hasProtectedTopic;
		curUsers = rhs.curUsers;
		invUsers = rhs.invUsers;
		chanOp = rhs.chanOp;
	}
	return *this;
}

int Channel::getUserLimits() const {
	return this->userLimit;
}

std::string	Channel::getName() const {
	return this->name;
}

std::string	Channel::getPassword() const {
	return this->password;
}

std::string	Channel::getTopic() const {
	return this->topic;
}

std::set<User*> Channel::getCurrentUsers() const {
	return this->curUsers;
}

std::set<User*> Channel::getUserInvitation() const {
	return this->invUsers;
}

std::set<User*> Channel::getChanOps() const {
	return this->chanOp;
}



void	Channel::setUserlimit(int value) {
	this->userLimit = value;
}

void	Channel::setPassword(std::string value) {
	this->password = value;
}

void	Channel::setTopic(std::string value) {
	this->topic = value;
}

std::pair<std::set<User*>::iterator, bool> Channel::addChanOps(User &value) {
	return this->chanOp.insert(&value);
}

std::pair<std::set<User*>::iterator, bool> Channel::addUser(User &value) {
	return this->curUsers.insert(&value);
}

std::pair<std::set<User*>::iterator, bool> Channel::addUserInvitation(User &value) {
	return this->invUsers.insert(&value);
}


std::string	Channel::replaceSpecialChar(std::string name) {
	std::string tmp = name;

	for (size_t i = 0; i < tmp.size(); i++)
	{
		if (tmp[i] == '{' || tmp[i] == '[')
			tmp[i] = ' ';
		else if (tmp[i] == '}' || tmp[i] == ']')
			tmp[i] = '\v';
		else if (tmp[i] == '|' || tmp[i] == '\\')
			tmp[i] = 8;
		}
	
	return tmp;	
}


std::size_t	Channel::removeChanOps(User &target) {
	return this->chanOp.erase(&target);
}

std::size_t	Channel::removeUser(User &target) {
	return this->curUsers.erase(&target);
}

std::size_t	Channel::removeUserInvitation(User &target) {
	return this->invUsers.erase(&target);
}

bool	Channel::operator<(const Channel &other) const {
	return this->getName() < other.getName();
}


User	*Channel::getChanOpByFd(int fd) const {
	for (std::set<User*>::const_iterator it = this->chanOp.begin(); it != this->chanOp.end(); it++) {
		if ((*it)->getFd().fd == fd) {
			return *it;
		}
	}

	return NULL;
}

User	*Channel::getChanOpByNickname(std::string nickname) const {
	for (std::set<User*>::const_iterator it = this->chanOp.begin(); it != this->chanOp.end(); it++) {
		std::string tmpNick = User::replaceSpecialChar((*it)->getNickName());
		if (tmpNick == nickname) {
			return *it;
		}
	}

	return NULL;
}

User	*Channel::getChanOpByUsername(std::string username) const {
	for (std::set<User*>::const_iterator it = this->chanOp.begin(); it != this->chanOp.end(); it++) {
		std::string tmpUsername = User::replaceSpecialChar((*it)->getUserName());
		if (tmpUsername == username) {
			return *it;
		}
	}

	return NULL;
}


User	*Channel::getConnectedUserByFd(int fd) const {
	for (std::set<User*>::const_iterator it = this->curUsers.begin(); it != this->curUsers.end(); it++) {
		if ((*it)->getFd().fd == fd) {
			return *it;
		}
	}

	return NULL;
}

User	*Channel::getConnectedUserByNickname(std::string nickname) const {
	for (std::set<User*>::const_iterator it = this->curUsers.begin(); it != this->curUsers.end(); it++) {
		std::string tmpNick = User::replaceSpecialChar((*it)->getNickName());
		if (tmpNick == nickname) {
			return *it;
		}
	}

	return NULL;
}

User	*Channel::getConnectedUserByUsername(std::string username) const {
	for (std::set<User*>::const_iterator it = this->curUsers.begin(); it != this->curUsers.end(); it++) {
		std::string tmpUsername = User::replaceSpecialChar((*it)->getNickName());
		if (tmpUsername == username) {
			return *it;
		}
	}

	return NULL;
}

User	*Channel::getInvitationUserByUsername(std::string username) const {
	for (std::set<User*>::const_iterator it = this->invUsers.begin(); it != this->invUsers.end(); it++) {
		std::string tmpUsername = User::replaceSpecialChar((*it)->getNickName());
		if (tmpUsername == username) {
			return *it;
		}
	}

	return NULL;
}

std::ostream &operator<<(std::ostream &out, Channel const &rhs) {
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

bool	Channel::hasForbbidenCharPassword(std::string pw) {
	if (pw.find_first_of(" \t\r\v\n#&!:$") != std::string::npos)
		return true;
	for (size_t i = 0; i < pw.size(); i++)
	{
		if (!isascii(pw[i]))
			return true;
	}
	return false;
}

bool	Channel::isProtectedByPassword() {
	if (this->password.size() > 1)
		return true;
	return false;
}

bool	Channel::getIsOnInvitationOnly() {
	return this->invitationOnly;
}


bool	Channel::getHasProtectedTopic() {
	return this->hasProtectedTopic;
}

void	Channel::setIsOnInvitationOnly(bool value) {
	this->invitationOnly = value;
}


void	Channel::setHasProtectedTopic(bool value) {
	this->hasProtectedTopic = value;
}	


void	Channel::sendActiveMode(User &user) {
	std::string activeFlag = "+";

	if (this->getIsOnInvitationOnly()) {
		activeFlag.append("i");
	}
	if (this->isProtectedByPassword()) {
		activeFlag.append("k");
	}
	if (this->getUserLimits() != 10) {
		activeFlag.append("l");
	}
	if (this->getHasProtectedTopic()) {
		activeFlag.append("t");
	}
	if (this->isProtectedByPassword()) {
		activeFlag.append(" ");
		activeFlag.append(this->getPassword());
	}
	if (this->getUserLimits() != 10) {
		activeFlag.append(" ");
		std::stringstream convertInt;

		convertInt << this->getUserLimits();
		
		std::string convertValue = convertInt.str();
		activeFlag.append(convertValue);
	}
	Message::modeSendActiveMode(user, *this, activeFlag);
}


void			printChannels(std::set<Channel*> channel) {
	for (std::set<Channel*>::iterator it = channel.begin(); it != channel.end(); ++it)
    	std::cout << *it;
}

