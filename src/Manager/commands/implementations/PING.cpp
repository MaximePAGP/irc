/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PING.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magrondi <magrondi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 01:51:33 by leye              #+#    #+#             */
/*   Updated: 2025/05/27 09:45:04 by magrondi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../CommandManager.hpp"
#include "../../../Class/Server/Server.hpp"

void  CommandManager::handlePing(std::string command, User &user) {
	(void)command;
	// Server const &server = Server::getServer();
	std::string pong = "PONG :server\r\n";
	Server::psend(user.getFd().fd, pong.c_str(), pong.length(), 0);
}

