#include <iostream>
#include <cstring>

static	int	handle_args(int argc, char **argv)
{
	if (argc < 3 || argc > 3)
	{
		std::cout << "./ircserv [port] [password] is expected" << std::endl;
		return 0;
	}
	if (strlen(argv[1]) == 0)
	{
		std::cout << "port shouldnt be empty" << std::endl;
		return 0;
	}
	if (strlen(argv[2]) == 0)
	{
		std::cout << "password shouldnt be empty" << std::endl;
		return 0;
	}
	return (1);
}

#include <set>

int main(int argc, char **argv)
{
	if (!handle_args(argc, argv))
		return 1;
	std::set<int> test;
	test.insert(50);
	std::cout << test.erase(50) << std::endl;

	return 0;
}