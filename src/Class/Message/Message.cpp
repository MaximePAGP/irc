#include "Message.hpp"

// 482  :You're not channel operator
void	Message::youreNotChanOp(std::string canalName, User &user) {
	std::string message = ":";

	message.append(ENV);
	message.append(" 482 ");
	message.append(user.getNickName());
	message.append(" #");
	message.append(canalName);
	message.append(" :You're not channel operator");
	message.append(END_CMD);
	
	send(user.getFd().fd, message.c_str(), message.size(), 0);
}


// 401   :No such nick/channel
void	Message::noSuchNickChannel(std::string target, User &user) {
	std::string message = ":";

	message.append(ENV);
	message.append(" 401 ");
	message.append(user.getNickName());
	message.append(" ");
	message.append(target);
	message.append(" :No such nick/channel");
	message.append(END_CMD);
	
	send(user.getFd().fd, message.c_str(), message.size(), 0);
}
 

void	Message::changedModeChan(std::string chanName, User &user, std::string flag) {
	std::string message = ":";

	message.append(ENV);
	message.append(" ");
	message.append(user.getNickName());
	message.append(" MODE #");
	message.append(chanName);
	message.append(" ");
	message.append(flag);
	message.append(END_CMD);
	
	send(user.getFd().fd, message.c_str(), message.size(), 0);
}


// 472 :is unknown mode char to me
void	Message::unknowFlag(User &user, std::string flag) {
	std::string message = ":";

	message.append(ENV);
	message.append(" 472 ");
	message.append(user.getNickName());
	message.append(" ");
	message.append(flag);
	message.append(" :is unknown mode char to me");
	message.append(END_CMD);
	
	send(user.getFd().fd, message.c_str(), message.size(), 0);
}


// 696  :Channel key contains forbidden characters
void	Message::chanPasswordForbiddenChar(std::string chanName, User &user, std::string password) {
	std::string message = ":";

	message.append(ENV);
	message.append(" 696 ");
	message.append(user.getNickName());
	message.append(" #");
	message.append(chanName);
	message.append(" +k ");
	message.append(password);
	message.append(" :Channel key contains forbidden characters");
	message.append(END_CMD);
	
	send(user.getFd().fd, message.c_str(), message.size(), 0);
}


// 696  :Channel key is too long
void	Message::chanPassordToLong(std::string chanName, User &user, std::string password) {
	std::string message = ":";

	message.append(ENV);
	message.append(" 696 ");
	message.append(user.getNickName());
	message.append(" #");
	message.append(chanName);
	message.append(" +k ");
	message.append(password);
	message.append(" :Channel key is too long");
	message.append(END_CMD);
	
	send(user.getFd().fd, message.c_str(), message.size(), 0);
}


// 431  :No nickname given
void	Message::nickNoParam(User &user, std::string nickname) {
	std::string message = ":";

	message.append(ENV);
	message.append(" 431 ");
	message.append(user.getNickName());
	message.append(" ");
	message.append(nickname);
	message.append(" :No nickname given");
	message.append(END_CMD);
	
	send(user.getFd().fd, message.c_str(), message.size(), 0);
}

// 432 :Erroneous nickname
void	Message::nickToLongParam(User &user, std::string nickname) {
	std::string message = ":";

	message.append(ENV);
	message.append(" 432 ");
	message.append(user.getNickName());
	message.append(" ");
	message.append(nickname);
	message.append(" :Erroneous nickname");
	message.append(END_CMD);
	
	send(user.getFd().fd, message.c_str(), message.size(), 0);
}


// 433   :Nickname is already in use
void	Message::nickAlreadyUsed(User &user, std::string nickname) {
	std::string message = ":";

	message.append(ENV);
	message.append(" 433 ");
	message.append(user.getNickName());
	message.append(" ");
	message.append(nickname);
	message.append(" :Nickname is already in use");
	message.append(END_CMD);
	
	send(user.getFd().fd, message.c_str(), message.size(), 0);
}

// 432  :Nickname is unavailable: Illegal characters
void	Message::nickIllegal(User &user, std::string nickname) {
	std::string message = ":";

	message.append(ENV);
	message.append(" 432 ");
	message.append(user.getNickName());
	message.append(" ");
	message.append(nickname);
	message.append(" :Nickname is unavailable: Illegal characters");
	message.append(END_CMD);
	
	send(user.getFd().fd, message.c_str(), message.size(), 0);
}

void	Message::nickSet(User &user) {
	std::string message = ":";

	message.append(ENV);
	message.append(" 001 ");
	message.append(user.getNickName());
	message.append(" :Nick has been set to ");
	message.append(user.getNickName());
	message.append(END_CMD);
	
	send(user.getFd().fd, message.c_str(), message.size(), 0);
}

void	Message::nickSetUpdated(User &user, std::string newNick) {
	std::string message = ":";

	message.append(ENV);
	message.append(" 001 ");
	message.append(user.getNickName());
	message.append(" :You've set your nickname to ");
	message.append(newNick);
	message.append(END_CMD);
	
	send(user.getFd().fd, message.c_str(), message.size(), 0);
}



// 461  USER :Not enough parameters
void	Message::userNoParam(User &user, std::string username) {
	std::string message = ":";

	message.append(ENV);
	message.append(" 461 ");
	message.append(user.getNickName());
	message.append(" ");
	message.append(username);
	message.append(" USER :Not enough parameters");
	message.append(END_CMD);
	
	send(user.getFd().fd, message.c_str(), message.size(), 0);
}


// 432   :Erroneous username
void	Message::userToLong(User &user, std::string username) {
	std::string message = ":";

	message.append(ENV);
	message.append(" 432 ");
	message.append(user.getNickName());
	message.append(" ");
	message.append(username);
	message.append(" :Erroneous username");
	message.append(END_CMD);
	
	send(user.getFd().fd, message.c_str(), message.size(), 0);
}


// 433   :Username is already in use
void	Message::userAlreadyTaken(User &user, std::string username) {
	std::string message = ":";

	message.append(ENV);
	message.append(" 433 ");
	message.append(user.getNickName());
	message.append(" ");
	message.append(username);
	message.append(" :Username is already in use");
	message.append(END_CMD);
	
	send(user.getFd().fd, message.c_str(), message.size(), 0);
}


// 432  :Username contains forbidden characters
void	Message::userForbiddenChar(User &user, std::string username) {
	std::string message = ":";

	message.append(ENV);
	message.append(" 432 ");
	message.append(user.getNickName());
	message.append(" ");
	message.append(username);
	message.append(" :Username contains forbidden characters");
	message.append(END_CMD);
	
	send(user.getFd().fd, message.c_str(), message.size(), 0);
}


// 432 :You may not reregister
void	Message::userCannotChange(User &user, std::string username) {
	std::string message = ":";

	message.append(ENV);
	message.append(" 432 ");
	message.append(user.getNickName());
	message.append(" ");
	message.append(username);
	message.append(" :You may not reregister");
	message.append(END_CMD);
	
	send(user.getFd().fd, message.c_str(), message.size(), 0);
}

void	Message::userSet(User &user) {
	std::string message = ":";

	message.append(ENV);
	message.append(" 001 USER ");
	message.append("Username has been set to ");
	message.append(user.getUserName());
	message.append(END_CMD);
	
	send(user.getFd().fd, message.c_str(), message.size(), 0);
}