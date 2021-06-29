/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlanca-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/28 15:34:06 by mlanca-c          #+#    #+#             */
/*   Updated: 2021/06/29 19:56:03 by mlanca-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

/*
*/
void	get_pid(int pid)
{
	ft_putstr_fd("PID: ", 1);
	ft_putnbr_fd(pid, 1);
	ft_putstr_fd("\n", 1);
}

void	handle_string(int pid, char c, int print)
{
	static char *message = 0;
	char		*temporary;
	int			i;

	if (print)
	{
		ft_putstr_fd(message, 1);
		free(message);
		if (kill(pid, SIGUSR2) == -1)
			exit(EXIT_FAILURE);
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
	if (kill(pid, SIGUSR1) == -1)
	{
		free(message);
		exit(EXIT_FAILURE);
	}
}

/*
*/
void	handler_sigusr(int signum, siginfo_t *info, void *context)
{
	static int	bit = 0;
	static char	c = 0xFF;

	(void)context;
	if (signum == SIGUSR1)
		c ^= 0x80 >> bit;
	else if (signum == SIGUSR2)
		c |= 0x80 >> bit;
	bit++;
	if (bit == 8)
	{
		if (c == '\0')
			handle_string(info->si_pid, 0, PRINT);
		else
			handle_string(info->si_pid, c, STORE);
		bit = 0;
		c = 0xFF;
	}
	else if (kill(info->si_pid, SIGUSR1) == -1)
		exit(EXIT_FAILURE);
}

/*
*/
int	main(void)
{
	struct sigaction	sa_signal;
	sigset_t 			block_mask;

	get_pid(getpid());
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
