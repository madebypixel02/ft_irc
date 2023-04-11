/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aparolar <aparolar@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 17:29:09 by aparolar          #+#    #+#             */
/*   Updated: 2023/04/05 17:43:41 by aparolar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <vector>
#include <string>
#include <sys/poll.h>
#include <sys/socket.h>

class Client;

class Client
{
	//Types
		typedef std::vector<pollfd>::iterator	pollfd_iter;

	private:
		//Properties
		int			_client_fd;
		int			_port;
		std::string	_hostname;

		//Methods

	public:
		//Constructors

		Client(int clientFd, const std::string& hostname, int port);

		//Destructors

		~Client();

		//Methods

		int			GetFd() { return _client_fd; }
		int			GetPort() { return _port; }
		std::string	GetHostname() const { return _hostname; }
		void		Write(const std::string& message) const;

};

#endif
