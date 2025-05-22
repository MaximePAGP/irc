#include "curl/curl.h"
#include "../Manager/commands/CommandManager.hpp"

size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string	content(std::string response) {
	size_t start = response.find("\"content\": \"");
	if (start == std::string::npos)
		return "Error API gpt";
	start += 12;
	size_t end = response.find("\",\n", start);
	if (end == std::string::npos)
		return "Error API gpt";
	return response.substr(start, end-start);
}

void		CommandManager::handleGpt(std::string param, User &user)
{
	CURL*				curl = curl_easy_init();
	struct curl_slist*	headers = NULL;
	std::string			json = "{";
	std::string			response;
	if (!curl)
		return ;

	json.append("\"model\":\"gpt-3.5-turbo\",");
	json.append("\"messages\":[{\"role\":\"user\",\"content\":\"");
	json.append(param);
	json.append("\"}],");
	json.append("\"max_tokens\":500");
	json.append("}");

	curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/chat/completions");
	headers = curl_slist_append(headers, "Content-Type: application/json");
	headers = curl_slist_append(headers, "Authorization: Bearer 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");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

	CURLcode error = curl_easy_perform(curl);
	if (error != CURLE_OK) {
		std::cerr << "Error curl : " << curl_easy_strerror(error) << std::endl;
		std::string	message = ":gpt PRIVMSG " + user.getNickName() + " Error curl" + END_CMD;
		return ;
	}

	std::string	message = ":gpt PRIVMSG " + user.getNickName() + " " + content(response) + END_CMD;
	send(user.getFd().fd, message.c_str(), message.size(), 0);

	curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
}
