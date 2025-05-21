#include "curl/curl.h"
#include <iostream>

void testCurl(void)
{
	CURL*	curl = curl_easy_init();

	if (curl)
	{
		std::cout << "test" << std::endl;
	}
}
