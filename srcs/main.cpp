/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aparolar <aparolar@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 11:42:05 by aparolar          #+#    #+#             */
/*   Updated: 2023/04/09 22:27:00 by aparolar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Sockets/BaseServer.hpp"
# include <iostream>

class Server : public BaseServer
{
private:
	
public:
	Server() : BaseServer("0.0.0.0", "8888") {}
	virtual void	OnClientConnected( BaseServerClientData& info) { std::cout << info.Address << std::endl;};
	virtual void	OnClientDisconnected(int client_fd) {};
	virtual void	OnClientData( BaseServerClientData& info) {};
};


int main(int argc, char **argv)
{
	Server	server;
	server.SetDebugMode();
	server.Run();
}