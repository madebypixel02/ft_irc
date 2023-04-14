/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aparolar <aparolar@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 11:21:12 by aparolar          #+#    #+#             */
/*   Updated: 2023/04/14 04:20:58 by aparolar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BaseServer.hpp"
#include <iostream>
#include <string>
#include <sstream>

BaseServer::BaseServer(const std::string &hostname, const std::string& port)
	: _hostname(hostname),
	  _port(port)
{
	_rawport = htons(StringToInt(port));
	_socket = -1;
	_server_fd = -1;
	_debug_mode = false;
	_working = false;
	CreateSocket();
}

BaseServer::~BaseServer()
{
}

void BaseServer::Run()
{
	pollfd server_pollfd = {_server_fd, POLLIN, 0};
	_pollfds.push_back(server_pollfd);
	_working = true;

	while (_working)
	{
		if (poll(_pollfds.begin().base(), _pollfds.size(), 10) < 0)
			throw std::runtime_error(ERR_SERVER_POLL);

		for (pollfd_iter it = _pollfds.begin(); it != _pollfds.end(); it++)
		{
			if (it->revents)
			{
				if ((it->revents & POLLIN) == POLLIN && it->fd == _server_fd)
				{
					OnConnect();
					break;
				}
				else if ((it->revents & POLLIN) == POLLIN)
				{
					if (!OnData(it->fd))
					{
						it--;
						continue;
					}
				}
			}
		}
	}
}

void BaseServer::OnConnect()
{
	Accept();
}

bool BaseServer::OnData(int client_fd)
{
	std::string data;
	int len;
	char buffer[RCV_BUFFER_SIZE];

	bzero(buffer, RCV_BUFFER_SIZE);
	len = recv(client_fd, buffer, RCV_BUFFER_SIZE, 0);

	if (!len && client_fd != _pollfds[0].fd)
	{
		OnDisconnect(client_fd);
		return false;
	}

	while (len > 0)
	{
		data.append(buffer);
		bzero(buffer, RCV_BUFFER_SIZE);
		len = recv(client_fd, buffer, RCV_BUFFER_SIZE, 0);
	}

	data.erase(data.end() - 1);

	BaseServerClientData info = GetClientInfoFromFd(client_fd);
	static_cast<std::string>(info.Data) = data;
	OnClientData(info);

	
	if (_debug_mode)
		std::cout << "From client fd [" << client_fd << "] data : [" << data << "]" << std::endl;
		
	return true;
}

void BaseServer::OnDisconnect(int client_fd)
{
	Disconnect(client_fd);
}

void BaseServer::CreateSocket()
{
	int server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sock < 0)
		throw std::runtime_error(ERR_SERVER_SOCKET);

	int value = 1;
	if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value)))
		throw std::runtime_error(ERR_SERVER_SETSOCKOPT);

	if (fcntl(server_sock, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error(ERR_SERVER_FCNTL);

	if (Bind(server_sock) < 0)
		throw std::runtime_error(ERR_SERVER_BIND);

	if (listen(server_sock, MAX_CLIENTS) < 0)
		std::runtime_error(ERR_SERVER_LISTEN);
	_server_fd = server_sock;
}

int BaseServer::Bind(int value)
{
	sockaddr_in addr = {};

	bzero((char *)&addr, sizeof(addr));
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_family = AF_INET;
	addr.sin_port = _rawport;

	if (bind(value, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		return (-1);
	return (value);
}

void BaseServer::Accept()
{
	int fd;
	sockaddr_in addr = {};
	socklen_t addr_len = sizeof(addr);
	char hostname[NI_MAXHOST];

	fd = accept(_server_fd, (struct sockaddr *)&addr, &addr_len);
	if (fd < 0)
		throw std::runtime_error(ERR_CLIENT_ACCEPT);

	pollfd pollfd = {fd, POLLIN, 0};
	_pollfds.push_back(pollfd);

	fcntl(fd, F_SETFL, O_NONBLOCK);

	// una vez llegados a este punto hay que añadir el nuevo cliente con sus datos al contenedor clientes
	BaseServerClientData	info(std::string(hostname), pollfd, "");
	_client_data.push_back(info);
	OnClientConnected(info);

	if (_debug_mode)
	{
		std::string proveMSG = "Hola, Bienvenidos al preservidor IRC.\n";
		std::cout << "sending Hello to socket [" << fd << "] with ip: " << GetClientAddress(&addr) << std::endl;
		if (send(fd, proveMSG.c_str(), proveMSG.length(), MSG_CONFIRM) < 0)
			std::cout << "server connection send error, put here an exception" << std::endl;
	}
}

void BaseServer::Disconnect(int client_fd)
{
	// qui cosas previas a hacer con las clases pertinentes en relacion al servidor irc
	OnClientDisconnected(client_fd);
	// Legados a este punto se elimina se recorre el vector en busca del pollfd del cliente que se desconecta
	// una vez encontrado se elimina del vector y se cierra el socket
	for (pollfd_iter it = _pollfds.begin() + 1; it != _pollfds.end(); it++)
	{
		if (_pollfds.size() > 1 && it !=_pollfds.begin() && it->fd == client_fd)
		{
			close(client_fd);
			_pollfds.erase(it);
			break;
		}
	}
}

std::string BaseServer::GetClientAddress(sockaddr_in *addr)
{
	char ip[INET_ADDRSTRLEN];
	int port;

	if (addr->sin_family == AF_INET)
	{
		port = ntohs(addr->sin_port);
		inet_ntop(AF_INET, &addr->sin_addr, ip, sizeof(ip));
	}
	/*else // En caso de querer saber datos de la ipv6
	{
		// AF_INET6
		struct sockaddr_in6 *s = (struct sockaddr_in6 *)&addr;
		port = ntohs(s->sin6_port);
		inet_ntop(AF_INET6, &s->sin6_addr, ip, sizeof ip);
	}*/
	return (std::string(ip));
}

int BaseServer::StringToInt(const std::string &string)
{
	std::istringstream ss(string);
	int value;

	ss >> value;
	return (value);
}


// Public Methods

void	BaseServer::SetDebugMode()
{
	_debug_mode = true;
}

void	BaseServer::UnsetDebugMode()
{
	_debug_mode = false;
}

BaseServerClientData& BaseServer::GetClientInfoFromFd(int client_fd)
{
	for (bcd_iter it = _client_data.begin(); it != _client_data.end(); it++)
	{
		if (it.base()->ClientPollFd.fd == client_fd)
		{
			return (*it);
		}
	}
	throw std::runtime_error("GetClientInfoFromFd error.");
}

void	BaseServer::Stop()
{
	_working = false;
}

