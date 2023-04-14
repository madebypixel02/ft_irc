/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aparolar <aparolar@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 11:03:14 by aparolar          #+#    #+#             */
/*   Updated: 2023/04/14 04:20:56 by aparolar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASE_SERVER_HPP
# define BASE_SERVER_HPP

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
# include "BaseServerClientData.hpp"

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

/*
	Notas: implementar y revisar POLLOUT y lo que devuelve recv
*/

// Base Class Server

class BaseServer
{
	// Public Typedef's
		typedef std::vector<pollfd>::iterator				pollfd_iter;
		typedef std::vector<BaseServerClientData>::iterator	bcd_iter;
	
	// Typename's

	private:

		// Properties
		bool								_debug_mode;
		int									_rawport;
		int									_socket;
		int									_server_fd;
		bool								_working;		

		std::vector<pollfd>					_pollfds;
		std::vector<BaseServerClientData>	_client_data;
		const std::string					_hostname;
		const std::string					_port;


		// Methods

		void					CreateSocket();
		int						Bind(int value);
		void					Accept();
		void					Disconnect(int client_fd);
		std::string				GetClientAddress(sockaddr_in *addr);
		int						StringToInt(const std::string& string);

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
		void	Stop();

			// Utility method's
			BaseServerClientData&	GetClientInfoFromFd(int client_fd);
	
};

#endif
