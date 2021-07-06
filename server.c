/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlanca-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/01 15:45:36 by mlanca-c          #+#    #+#             */
/*   Updated: 2021/07/06 15:55:03 by mlanca-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

/*
** This function is called in case an error occurred in a kill() system call in
** server. It frees 'str' if not null and before exiting the program with
** EXIT_FAILURE it outputs a error message, and tries to send SIGUSR2 to client
** signaling an error.
*/
void	error(int pid, char *str)
{
	if (str)
		free(str);
	ft_putstr_color_fd(ANSI_COLOR_RED,
		"server: unexpected error.\n", 2);
	kill(pid, SIGUSR2);
	exit(EXIT_FAILURE);
}

/*
** This function is called once server received the full string - 'message' from
** client. The print_string() function outputs 'message' and then frees it
** returning zero.
*/
char	*print_string(char *message)
{
	ft_putstr_fd(message, 1);
	free(message);
	return (NULL);
}

/*
** This function is called every time server receives a signal - SIGUSR1 or 
** SIGUSR2 from client.
** SIGUSR1 represents a 0; SIGUSR2 represents 1. By getting these signals from
** client server is able to recreate the string - receiving it bit-by-bit -
** using bitwise operations.
**
** @line		- The function starts by adding the bit - 0 or 1 depending on
** 				the signal it received from client - in a static variable type
** 				char 'c'.
**
** @line		- Once 8 signals are received - handler_sigusr() is
** 				called 8 times or bits == 8 - then the character is complete and
** 				is then added to another static variable type char * - 'message'
** 				
** @line		- if the received character represents the null character, then
** 				nothing more is added to 'message' and the function
** 				print_string() is called to print the message, that is then set
** 				back to null.
**
** @line		- After receiving the signal and successfully storing the bit
** 				in the string, server sends a SIGUSR1 signal to client as
** 				confirmation it received the bit it send and that is ready for
** 				another.
*/
void	handler_sigusr(int signum, siginfo_t *info, void *context)
{
	static char	c = 0xFF;
	static int	bits = 0;
	static int	pid = 0;
	static char	*message = 0;

	(void)context;
	if (info->si_pid)
		pid = info->si_pid;
	if (signum == SIGUSR1)
		c ^= 0x80 >> bits;
	else if (signum == SIGUSR2)
		c |= 0x80 >> bits;
	if (++bits == 8)
	{
		if (c)
			message = ft_straddc(message, c);
		else
			message = print_string(message);
		bits = 0;
		c = 0xFF;
	}
	if (kill(pid, SIGUSR1) == -1)
		error(pid, message);
}

/*
** This is the main function of server.
**
** @line 115-123	- It starts by setting up the function that will receive the
** 					signals from a client - sigaction() used instead of signal()
** 					in order to get the parameter 'info->si_pid' the pid of the
** 					sender - client.
**
** @line 124-127	- Once the setting up is done, server uses the function
** 					getpid() to output to the user its PID. This will later be
** 					used to compile client.
**
** @line 128-129	- The function then enters an infinite pause() loop waiting
** 					for signals from client.
*/
int	main(void)
{
	struct sigaction	sa_signal;
	sigset_t			block_mask;

	sigemptyset(&block_mask);
	sigaddset(&block_mask, SIGINT);
	sigaddset(&block_mask, SIGQUIT);
	sa_signal.sa_handler = 0;
	sa_signal.sa_flags = SA_SIGINFO;
	sa_signal.sa_mask = block_mask;
	sa_signal.sa_sigaction = handler_sigusr;
	sigaction(SIGUSR1, &sa_signal, NULL);
	sigaction(SIGUSR2, &sa_signal, NULL);
	ft_putstr_color_fd(ANSI_COLOR_GREEN,
		"PID: ", 1);
	ft_putnbr_fd(getpid(), 1);
	ft_putstr_fd("\n", 1);
	while (1)
		pause();
}
