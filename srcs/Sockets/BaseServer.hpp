/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aparolar <aparolar@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 11:03:14 by aparolar          #+#    #+#             */
/*   Updated: 2023/04/10 22:10:35 by aparolar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

class Server;

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/poll.h>
# include <poll.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <strings.h>
# include <string>
# include <vector>
# include <exception>

# define	nullprt					0
# define	MAX_CLIENTS				1000
# define	RCV_BUFFER_SIZE			5000
# define	ERR_SERVER_POLL			"BaseServer main socket poll run-time error."
# define	ERR_SERVER_SOCKET		"BaseServer main socket run-time error."
# define	ERR_SERVER_SETSOCKOPT	"BaseServer main socket setsockopt run-time error."
# define	ERR_SERVER_FCNTL		"BaseServer main socket fcntl run-time error."
# define	ERR_SERVER_BIND			"BaseServer main socket bind run-time error."
# define	ERR_SERVER_LISTEN		"BaseServer main socket listen run-time error."
# define	ERR_CLIENT_ACCEPT		"BaseServer client socket accept run-time error."

// Struct to events

struct BaseServerClientData
{
	public:

		BaseServerClientData( const std::string address, const pollfd pfd, const std::string data, bool good_info = false )
			:	Address(address),
				ClientPollFd(pfd),
				GoodInfo(good_info),
				Data(data) {};

		const std::string	Address;
		const pollfd		ClientPollFd;
		const std::string	Data;
		bool				GoodInfo;
};

// Base Class Server

class BaseServer
{
		typedef std::vector<pollfd>::iterator				pollfd_iter;
		typedef std::vector<BaseServerClientData>::iterator	bcd_iter;

	private:
		//typenames

		//Properties
		bool								_debug_mode;
		int									_rawport;
		int									_socket;
		int									_server_fd;
				
		std::vector<pollfd>					_pollfds;
		std::vector<BaseServerClientData>	_client_data;
		const std::string					_hostname;
		const std::string					_port;


		//Methods

		void					CreateSocket();
		int						Bind(int value);
		void					Accept();
		void					Disconnect(int client_fd);
		std::string				GetClientAddress(sockaddr_in *addr);
		int						StringToInt(const std::string& string);
		BaseServerClientData&	GetClientInfoFromFd(int client_fd);

		//Connection
		void		OnConnect();
		bool		OnData(int client_fd);
		void		OnDisconnect(int client_fd);

	public:

		// Constructors
		BaseServer()
			:	_hostname("0.0.0.0"),
				_port("42")
		{}
		
		explicit BaseServer(const std::string& hostname, const std::string& port);

		// Destructors
		virtual ~BaseServer();

		// General methods
		virtual void	Run();

		// Event methods
		
		virtual void	OnClientConnected( BaseServerClientData& info) = 0;
		virtual void	OnClientDisconnected(int client_fd) = 0;
		virtual void	OnClientData( BaseServerClientData& info) = 0;

		// Properties access methods
		void	SetDebugMode();
		void	UnsetDebugMode();
	
};

#endif
