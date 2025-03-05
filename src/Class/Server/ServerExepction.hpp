#ifndef SEVER_EXEPCTION_CLASS_HPP
#define SEVER_EXEPCTION_CLASS_HPP

#include <exception>
	
class ServerExepction
{
	public:
		ServerExepction() {};
		virtual ~ServerExepction() {};
		class CannotBindPortException : public std::exception
		{
			public:
				virtual const char* what() const throw()
				{
					return "Bind portname failed";
				}
		};
		class CannotListenException : public std::exception
		{
			public:
				virtual const char* what() const throw()
				{
					return "Listen portname failed";
				}
		};
		class SocketCreationException : public std::exception
		{
			public:
				virtual const char* what() const throw()
				{
					return "Socket creation failed";
				}
		};
};




#endif