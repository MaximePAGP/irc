#include <cstring>
#include <cstdlib>

#include "Class/Server/Server.hpp"


int	checkPassingArguments(int argc, char **argv);

int main(int argc, char **argv)
{
	if (!checkPassingArguments(argc, argv))
		return 1;



	return 0;
}