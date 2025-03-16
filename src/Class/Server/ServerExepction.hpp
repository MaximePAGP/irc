#ifndef SEVER_EXEPCTION_CLASS_HPP
#define SEVER_EXEPCTION_CLASS_HPP

#include <exception>
	
class ServerExepction
{
	public:
		ServerExepction() {};
		virtual ~ServerExepction() {};
		class PortOutOfRangeException : public std::exception
		{
			public:
				virtual const char* what() const throw()
				{
					return "Portname should be in range to 1 and 65535";
				}
		};
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
		class CannotSetFdOptionsException : public std::exception
		{
			public:
				virtual const char* what() const throw()
				{
					return "FCNTL failed";
				}
		};
		class CannotSetSocketOptionException : public std::exception
		{
			public:
				virtual const char* what() const throw()
				{
					return "Socket options failed";
				}
		};
};




#endif