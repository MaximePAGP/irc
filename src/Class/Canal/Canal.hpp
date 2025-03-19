#ifndef CANAL_CLASS_HPP
#define CANAL_CLASS_HPP


#include <string>
#include <poll.h>
#include <set>
#include "../Flag/Flag.hpp"
#include "../User/User.hpp"

void	printUsers(std::set<User*> users);

class Canal
{
	private:
		Canal();

		struct	pollfd			fd;
		int						userLimit;
		Flag					flags;
		std::string	const		name;
		std::string				password;
		std::string				topic;
		std::set<User*>			curUsers;
		std::set<User*>			invUsers;
		std::set<User*>			chanOp;
	public:
		Canal(struct pollfd &fd, std::string &name);
		virtual ~Canal();
		Canal(Canal const &copy);
		Canal &operator=(Canal const &rhs);

		struct	pollfd	getFd() const;
		int				getUserLimits() const;
		Flag			getFlag() const;
		std::string		getName() const;
		std::string		getPassword() const;
		std::string		getTopic() const;
		std::set<User*>	getCurrentUsers() const;
		std::set<User*>	getUserInvitation() const;
		std::set<User*>	getChanOps() const;

		void			setFd(const struct pollfd &value);
		void			setUserlimit(int value);
		void			setPassword(std::string value);
		void			setTopic(std::string value);

		static	std::string	replaceSpecialChar(std::string param);

		std::pair<std::set<User*>::iterator, bool>
						addChanOps(User &value);
		std::pair<std::set<User*>::iterator, bool>
						addUserInvitation(User &value);
		std::pair<std::set<User*>::iterator, bool>
						addUser(User &value);
		
		std::size_t		removeChanOps(User &value);
		std::size_t		removeUserInvitation(User &value);
		std::size_t		removeUser(User &value);
	
		bool			operator<(const Canal &other) const;
};

std::ostream &operator<<(std::ostream &out, Canal const &rhs);
void	printCanals(std::set<Canal*>);


#endif