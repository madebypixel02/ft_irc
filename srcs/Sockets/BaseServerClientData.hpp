/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseServerClientData.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aparolar <aparolar@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 04:00:16 by aparolar          #+#    #+#             */
/*   Updated: 2023/04/14 04:02:30 by aparolar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASE_SERVER_CLIENT_DATA_HPP
# define BASE_SERVER_CLIENT_DATA_HPP

# include <string>
# include <poll.h>
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

#endif
