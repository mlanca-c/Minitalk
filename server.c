/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlanca-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/28 15:34:06 by mlanca-c          #+#    #+#             */
/*   Updated: 2021/06/30 13:02:54 by mlanca-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

/*
** This function prints the PID of server using getpid() function.
*/
void	get_pid(void)
{
	ft_putstr_fd("PID: ", 1);
	ft_putnbr_fd(getpid(), 1);
	ft_putstr_fd("\n", 1);
}

/*
*/
void	handle_string(int pid, char c, int status)
{
	static char *message = 0;
	char		*temporary;
	int			i;

	if (status)
	{
		ft_putstr_fd(message, 1);
		message = 0;
		free(message);
		if (kill(pid, SIGUSR2) == -1)
			exit(EXIT_FAILURE);
		return ;
	}
	if (!message)
	{
		message = (char *)malloc(sizeof(char) * 2);
		message[0] = c;
		message[1] = '\0';
		if (kill(pid, SIGUSR1) == -1)
		{
			free(message);
			exit(EXIT_FAILURE);
		}
		return ;
	}
	temporary = message;
	message = (char *)malloc(sizeof(char) * (ft_strlen(message) + 2));
	i = -1;
	if (temporary)
	{
		while (temporary[++i])
			message[i] = temporary[i];
		free(temporary);
	}
	message[i++] = c;
	message[i] = '\0';
	kill(pid, SIGUSR1);
}

/*
 * This function is called when the program receives either SIGUSR1 or SIGUSR2.
 * It has two static variables:
 * 		int bit	- is the number of signals - representing bits - that the
 * 		program has received.
 * 		char c	- is the character created by the 8 signals server receives.
 * Every 8 bits the program receives a character c is formed. Once it's formed
 * handle_string() function is called with the status 'STORE', and that
 * character is added to 'message'- string client sends server. If the character
 * is null (if the program receives 8 SIGUSR1 signals - 0 - on a row), then
 * handle_string() function is called with the status 'PRINT', and the program
 * knows that the string was fully sent by client.
*/
void	handler_sigusr(int signum, siginfo_t *info, void *context)
{
	static int	bit = 0;
	static char	c = 0xFF;

	(void)context;
	if (signum == SIGUSR1)
		c ^= 0x80 >> bit % 8;
	else if (signum == SIGUSR2)
		c |= 0x80 >> bit % 8;
	bit++;
	if (bit % 8 == 0)
	{
		if (c == '\0')
			handle_string(info->si_pid, 0, PRINT);
		else
			handle_string(info->si_pid, c, STORE);
		c = 0xFF;
	}
	else
		kill(info->si_pid, SIGUSR1);
}

/*
** This is the main function of client. First it outputs it's process ID - PID
** using the get_pid() function. Then waits for a signal from client - SIGUSR1
** representing 0 or SIGUSR2 representing 1. Once it receives a signal the
** function handler_sigusr() is called.
*/
int	main(void)
{
	struct sigaction	sa_signal;
	sigset_t 			block_mask;

	get_pid();
	sigemptyset(&block_mask);
	sigaddset(&block_mask, SIGINT);
	sigaddset(&block_mask, SIGQUIT);
	sa_signal.sa_handler = 0;
	sa_signal.sa_flags = SA_SIGINFO;
	sa_signal.sa_mask = block_mask;
	sa_signal.sa_sigaction = handler_sigusr;
	sigaction(SIGUSR1, &sa_signal, NULL);
	sigaction(SIGUSR2, &sa_signal, NULL);
	while (1)
		;
}
