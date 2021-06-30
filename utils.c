/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlanca-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/30 15:27:03 by mlanca-c          #+#    #+#             */
/*   Updated: 2021/06/30 17:04:42 by mlanca-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

/*
** This function is called when server received the first 8 bits. What this
** function does is it creates message, that will result in 'c' in the first
** position and the null character in the second. and then sends SIGUSR1 to
** client asking for more bits.
*/
char	*init_string(int pid, char *message, char c)
{
	ft_putstr_fd(ANSI_COLOR_BLUE, 1);
	ft_putstr_fd("CLIENT\n", 1);
	ft_putstr_fd(ANSI_COLOR_RESET, 1);
	message = (char *)malloc(sizeof(char) * 2);
	if (!message)
		exit(EXIT_FAILURE);
	message[0] = c;
	message[1] = '\0';
	kill_string(pid, message);
	return (message);
}

/*
** This function is called when server received another 8 bits from client and a
** new character is created - 'c'.
** What store_string() function does is it adds to the already existing string
** 'message' the character 'c'; frees the old message and returns the new one
** back to handle_string().
*/
char	*store_string(int pid, char *message, char c)
{
	char	*temporary;
	int		i;

	temporary = message;
	message = (char *)malloc(sizeof(char) * (ft_strlen(message) + 2));
	if (!message)
	{
		free(temporary);
		exit(EXIT_FAILURE);
	}
	i = -1;
	while (temporary[++i])
		message[i] = temporary[i];
	free(temporary);
	message[i++] = c;
	message[i] = '\0';
	kill_string(pid, message);
	return (message);
}

/*
** This function is called when there are no more signals to be received from
** client - string message is complete. When this happens the program prints
** message, and then sends SIGUSR2 to client to signal the success of the 
** operation.
*/
char	*print_string(int pid, char *message)
{
	ft_putstr_fd(message, 1);
	ft_putstr_fd(ANSI_COLOR_GREEN, 1);
	ft_putstr_fd("\nserver: operation successful.\n", 1);
	ft_putstr_fd(ANSI_COLOR_RESET, 1);
	message = 0;
	free(message);
	if (kill(pid, SIGUSR2) == -1)
		exit(EXIT_FAILURE);
	return (NULL);
}

/*
** This function is called when server wants to send SIGUSR1 to client asking
** for the next bit. If the kill() system call somehow fails, then 'message' is
** freed and the program exits.
*/
char	*kill_string(int pid, char *message)
{
	if (kill(pid, SIGUSR1) == -1)
	{
		message = 0;
		free(message);
		ft_putstr_fd(ANSI_COLOR_RED, 1);
		ft_putstr_fd("\nserver: unexpected error.\n", 1);
		ft_putstr_fd(ANSI_COLOR_RESET, 1);
		exit(EXIT_FAILURE);
	}
	return (message);
}
