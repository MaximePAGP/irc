#include <iostream>
#include <cstring>
#include <cstdlib>

static	bool	isDigitOnly(char *str) {
	if (!str)
		return false;
	for (size_t i = 0; i < std::strlen(str); ++i) {
		if (!std::isdigit(str[i]))
		{
			std::cout << "port need to be in digits" << std::endl;
			return false;
		}
	}
	return true;
}

	bool	portnameIsEmpty(char *portname)
{
	if (strlen(portname) == 0)
	{
		std::cout << "port shouldnt be empty" << std::endl;
		return true;
	}
	return false;
}

static	bool	passwordIsEmpty(char *password)
{
	if (strlen(password) == 0)
	{
		std::cout << "password shouldnt be empty" << std::endl;
		return true;
	}
	return false;
}

static	bool	hasCorrectParamsNumber(int argc) {
	if (argc < 3 || argc > 3)
	{
		std::cout << "./ircserv [port] [password] is expected" << std::endl;
		return false;
	}
	return true;
}

static	bool	isValidPort(int portname) {
	if (portname < 1 || portname > 65535) {
		std::cout << "Port should be in the range to 1 and 65535" << std::endl;
		return false;
	}
	return true;
}

int	checkPassingArguments(int argc, char **argv)
{
	if (!hasCorrectParamsNumber(argc))
		return 0;
	if (!isValidPort(std::atoi(argv[1])))
		return (0);
	if (!isDigitOnly(argv[1]))
		return 0;
	if (portnameIsEmpty(argv[1]) || passwordIsEmpty(argv[2]))
		return 0;

	return (1);
}
