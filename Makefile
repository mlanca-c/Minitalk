# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mlanca-c <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/06/22 12:44:59 by mlanca-c          #+#    #+#              #
#    Updated: 2021/06/23 13:03:03 by mlanca-c         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

	# Minitalk (Mandatory Part) Variables #
CLIENT	=	client
SERVER	=	server
SRC_C	=	client.c
SRC_S	=	server.c

	# Includes flag for compilation #
INC		= -I. -Ift_printf/ -Ift_printf/libft/ -Ift_printf/libft/stack

	# library #
LIBFT	=	ft_printf/libftprintf.a

	# Compiling Variables #
CC		=	gcc
CFLAG	=	-Wall -Wextra -Werror
RM		=	rm -f

	# Colors #
GREEN		=	\e[38;5;118m
YELLOW		=	\e[38;5;226m
RESET		=	\e[0m
_SUCCESS	=	[$(GREEN)SUCCESS$(RESET)]
_INFO		=	[$(YELLOW)INFO$(RESET)]

	# Debugger #
ifeq ($(DEBUG), 1)
	D_FLAG	=	-g
endif

	# Fsanitize #
ifeq ($(SANITIZE), 1)
	D_FLAG	=	-fsanitize=address -g
endif

mandatory:	$(CLIENT) $(SERVER)
bonus:		$(BONUS)

m : mandatory
b : bonus


$(CLIENT):
	@ $(MAKE) re -C ./ft_printf
	@ $(CC) $(D_FLAG) $(CFLAG) $(SRC_C) $(LIBFT) $(INC) -o $(CLIENT)
	@printf "$(_SUCCESS) client created.\n"

$(SERVER):
	@ $(MAKE) re -C ./ft_printf
	@ $(CC) $(D_FLAG) $(CFLAG) $(SRC_S) $(LIBFT) $(INC) -o $(SERVER)
	@printf "$(_SUCCESS) server created.\n"

all: $(CLIENT) $(SERVER)

clean:
	@ $(MAKE) clean -C ./ft_printf
	@ $(RM) $(CLIENT) $(SERVER)
	@printf "$(_INFO) client and server removed.\n"

fclean: clean
	@ $(MAKE) fclean -C ./ft_printf

re: fclean all

PHONY: all clean fclean re mandatory bonus m b
