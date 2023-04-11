# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aparolar <aparolar@student.42madrid.com>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/15 22:28:35 by aparolar          #+#    #+#              #
#    Updated: 2023/04/09 01:09:47 by aparolar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	ircserv

CC			=	g++
LDFLAGS		=	-fsanitize=address -g3
42FLAGS		=	-Wall -Wextra -Werror
CFLAGS		=	-std=c++98 $(LDFLAGS) #$(42FLAGS)

SRCS		=	srcs/main.cpp \
				srcs/Sockets/BaseServer.cpp

OBJS		=	$(SRCS:.cpp=.o)

$(NAME)	:	$(OBJS)
	@$(CC) $(CFLAGS) $(LDFLAGS) $(SRCS) -o $(NAME)

all		:	$(NAME)

clean	:
			@rm -rf $(OBJS);

fclean	:	clean
			@rm -rf $(NAME)

re		:	fclean all

.PHONY	:	all clean fclean re