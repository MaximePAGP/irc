/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PING.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leye <leye@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 01:51:33 by leye              #+#    #+#             */
/*   Updated: 2025/03/28 01:53:29 by leye             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../CommandManager.hpp"
#include "../../../Class/Server/Server.hpp"

void  CommandManager::handlePing(std::string command, User &user) {
	(void)command;
	Server const &server = Server::getServer();
	std::string pong = ":" + server.getPassword() + " PONG " + server.getPassword() + " " + user.getNickName() + "\r\n";
	send(user.getFd().fd, pong.c_str(), pong.length(), 0);
}