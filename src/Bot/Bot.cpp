#include "curl/curl.h"
#include "../Manager/commands/CommandManager.hpp"

size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void		handleGpt(std::string param, User &user)
{
	CURL*				curl = curl_easy_init();
	struct curl_slist*	headers = NULL;
	std::string			json = "{\"model\":\"gpt-3.5-turbo\",\"messages\":[{\"role\":\"user\",\"content\":\"Bonjour\"}],\"max_tokens\":30}";
	std::string			response;
	if (!curl)
		return ;

	(void)param;
	(void)user;

	curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/chat/completions");
	headers = curl_slist_append(headers, "Content-Type: application/json");
	headers = curl_slist_append(headers, "Authorization: Bearer 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");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

	curl_easy_perform(curl);

	std::cout << response << std::endl;

	curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

	/*
	if (curl)
	{
		std::cout << "test" << std::endl;
	}
	*/
}

