#include "Message.hpp"

// 482  :You're not channel operator
void	Message::youreNotChanOp(std::string const canalName, User const &user) {
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
void	Message::noSuchNickChannel(std::string const target, User const &user) {
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
 

void	Message::changedModeChan(std::string const &chanOp, std::string const chanName, User const &user, std::string const flag) {
	std::string message = ":";

	message.append(chanOp);
	message.append(" MODE #");
	message.append(chanName);
	message.append(flag);
	message.append(END_CMD);
	
	send(user.getFd().fd, message.c_str(), message.size(), 0);
}


// 472 :is unknown mode char to me
void	Message::unknowFlag(User const &user, std::string const flag) {
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
void	Message::chanPasswordForbiddenChar(std::string const chanName, User const &user, std::string const password) {
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
void	Message::chanPassordToLong(std::string const chanName, User const &user, std::string const password) {
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
void	Message::nickNoParam(User const &user, std::string const nickname) {
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
void	Message::nickToLongParam(User const &user, std::string const nickname) {
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
void	Message::nickAlreadyUsed(User const &user, std::string const nickname) {
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
void	Message::nickIllegal(User const &user, std::string const nickname) {
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

void	Message::nickSet(User const &user) {
	std::string message = ":";

	message.append(ENV);
	message.append(" 001 ");
	message.append(user.getNickName());
	message.append(" :Nick has been set to ");
	message.append(user.getNickName());
	message.append(END_CMD);
	
	send(user.getFd().fd, message.c_str(), message.size(), 0);
}

void	Message::nickSetUpdated(User const &user, std::string const newNick) {
	std::string message = ":";

	message.append(user.getNickName());
	message.append(" NICK :");
	message.append(newNick);
	message.append(END_CMD);

	send(user.getFd().fd, message.c_str(), message.size(), 0);
}




// 432   :Erroneous username
void	Message::userToLong(User const &user, std::string const username) {
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
void	Message::userAlreadyTaken(User const &user, std::string const username) {
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
void	Message::userForbiddenChar(User const &user, std::string const username) {
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


// 462 :You may not reregister
void	Message::userCannotChange(User const &user, std::string const username) {
	std::string message = ":";

	message.append(ENV);
	message.append(" 462 ");
	message.append(user.getNickName());
	message.append(" ");
	message.append(username);
	message.append(" :You may not reregister");
	message.append(END_CMD);
	
	send(user.getFd().fd, message.c_str(), message.size(), 0);
}

void	Message::userSet(User const &user) {
	std::string message = ":";

	message.append(ENV);
	message.append(" 001 USER ");
	message.append("Username has been set to ");
	message.append(user.getUserName());
	message.append(END_CMD);
	
	send(user.getFd().fd, message.c_str(), message.size(), 0);
}


// 403 :No such channel
void	Message::modeNotSuchChannel(User const &user, const std::string canalName) {
	std::string message = ":";

	message.append(ENV);
	message.append(" 403 ");
	message.append(user.getUserName());
	message.append(" #");
	message.append(canalName);
	message.append(" :No such channel");
	message.append(END_CMD);
	
	send(user.getFd().fd, message.c_str(), message.size(), 0);
}


void	Message::topicSetTopic(User const &user, Channel const &canal) {
	std::string message = ":";

	message.append(user.getNickName());	
	message.append(" TOPIC #");
	message.append(canal.getName());
	message.append(" :");
	message.append(canal.getTopic());
	message.append(END_CMD);
	
	send(user.getFd().fd, message.c_str(), message.size(), 0);
}

// 331 :No topic is set
void	Message::topicNoTopic(User const &user, std::string const canalName) {
	std::string message = ":";

	message.append(ENV);
	message.append(" 331 ");
	message.append(user.getNickName());
	message.append(" #");
	message.append(canalName);
	message.append(" :No topic is set");
	message.append(END_CMD);
	
	send(user.getFd().fd, message.c_str(), message.size(), 0);
}

// 332
void	Message::topicGetTopic(User const &user, Channel const &canal) {
	std::string message = ":";

	message.append(ENV);
	message.append(" 332 ");
	message.append(user.getNickName());
	message.append(" #");
	message.append(canal.getName());
	message.append(" :");
	message.append(canal.getTopic());
	message.append(END_CMD);
	
	send(user.getFd().fd, message.c_str(), message.size(), 0);
}


// 421 :Unknown command
void	Message::noSuchCommand(User const &user, std::string const &command) {
	std::string message = ":";

	message.append(ENV);
	message.append(" 421 ");
	message.append(user.getNickName());
	message.append(" ");
	message.append(command);
	message.append(" :Unknown command");
	message.append(END_CMD);
	
	send(user.getFd().fd, message.c_str(), message.size(), 0);
}


// :server.example 414 YourNickname :Message too long
void	Message::commandToLong(User const &user) {
	std::string message = ":";

	message.append(ENV);
	message.append(" 414 ");
	message.append(user.getNickName());
	message.append(" ");
	message.append(" :Message too long");
	message.append(END_CMD);
	
	send(user.getFd().fd, message.c_str(), message.size(), 0);
}

// 443
void	Message::alreadyOnChannel(User const &user, Channel const &canal) {
	std::string message = ":";
	message.append(ENV);
	message.append(" 443 ");
	message.append(user.getNickName());
	message.append(" #");
	message.append(canal.getName());
	message.append(" :is already on channel");
	message.append(END_CMD);

	send(user.getFd().fd, message.c_str(), message.size(), 0);
}



// 461
void	Message::notEnoughParams(User const &user, std::string const &command) {
	std::string message = ":";
	message.append(ENV);
	message.append(" 461 ");
	message.append(user.getNickName());
	message.append(" ");
	message.append(command);
	message.append(" :Not enough parameters");
	message.append(END_CMD);

	send(user.getFd().fd, message.c_str(), message.size(), 0);
}


void	Message::kickSucces(User const &user, Channel const &channel, std::string const &target, std::string const reason) {
	std::string message = ":";
	message.append(user.getNickName());
	message.append(" KICK #");
	message.append(channel.getName());
	message.append(" ");
	message.append(target);
	if (!reason.empty()) {
		message.append(" ");
		message.append(reason);
	}
	message.append(END_CMD);

	send(user.getFd().fd, message.c_str(), message.size(), 0);
}





// 442
void	Message::partYoureNotInChan(User const &user, std::string const &chanName) {
	std::string message = ":";
	message.append(ENV);
	message.append(" 442 ");
	message.append(chanName);
	message.append(" :You're not on that channel");
	message.append(END_CMD);

	send(user.getFd().fd, message.c_str(), message.size(), 0);
}


void	Message::partNotification(User const &user,  std::string const &chanName, std::string const &userLeft) {
	std::string message = ":";
	message.append(userLeft);
	message.append(" PART #");
	message.append(chanName);
	message.append(END_CMD);

	send(user.getFd().fd, message.c_str(), message.size(), 0);
}


// 324
void	Message::modeSendActiveMode(User const &user,  Channel &channel, std::string const activeFlags) {
	std::string message = ":";

	message.append(ENV);
	message.append(" 324 ");
	message.append(user.getNickName());
	message.append(" #");
	message.append(channel.getName());
	message.append(" ");
	message.append(activeFlags);
	message.append(END_CMD);

	send(user.getFd().fd, message.c_str(), message.size(), 0);
}

void	Message::noRegistered(User const &user) {
	std::string message = ":";

	message.append(ENV);
	message.append(" 451 ");
	message.append(" * ");
	message.append(": You have not registered");

	send(user.getFd().fd, message.c_str(), message.size(), 0);
}
