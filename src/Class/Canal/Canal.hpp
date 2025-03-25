#ifndef CANAL_CLASS_HPP
#define CANAL_CLASS_HPP


#include <string>
#include <poll.h>
#include <set>
#include "../User/User.hpp"

void	printUsers(std::set<User*> users);

class Canal
{
	private:
		Canal();
		int						userLimit; // flag +l 
		bool					invitationOnly; // flag +i
		bool					hasProtectedTopic; // flag +t
		std::string	const		name;
		std::string				password; // flag +k
		std::string				topic; // flag +t
		std::set<User*>			curUsers;
		std::set<User*>			invUsers;
		std::set<User*>			chanOp;
	public:
		Canal(std::string name);
		virtual ~Canal();
		Canal(Canal const &copy);
		Canal &operator=(Canal const &rhs);

		int				getUserLimits() const;
		std::string		getName() const;
		std::string		getPassword() const;
		std::string		getTopic() const;
		std::set<User*>	getCurrentUsers() const;
		std::set<User*>	getUserInvitation() const;
		std::set<User*>	getChanOps() const;
		bool			getIsOnInvitationOnly();
		bool			getHasProtectedTopic();

		void			setUserlimit(int value);
		void			setIsOnInvitationOnly(bool value);
		void			setPassword(std::string value);
		void			setTopic(std::string value);
		void			setHasProtectedTopic(bool value);

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
	
		User			*getChanOpByNickname(std::string nickname) const;
		User			*getChanOpByUsername(std::string username) const;
		User			*getChanOpByFd(int fd) const;

		User			*getConnectedUserByNickname(std::string nickname) const;
		User			*getConnectedUserByUsername(std::string username) const;
		User			*getConnectedUserByFd(int fd) const;

		static	bool	hasForbbidenCharPassword(std::string pw);

		bool			isProtectedByPassword(); // used on +k flag !! PASSWORD IS NOT REQUIRED IF USER IS INVITED
		bool			operator<(const Canal &other) const;
};

std::ostream &operator<<(std::ostream &out, Canal const &rhs);
void	printCanals(std::set<Canal*>);


#endif