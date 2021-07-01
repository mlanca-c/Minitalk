/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlanca-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/01 15:45:36 by mlanca-c          #+#    #+#             */
/*   Updated: 2021/07/01 17:55:57 by mlanca-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	error(int pid, char *str)
{
	if (str)
		free(str);
	ft_putstr_color_fd(ANSI_COLOR_RED,
		"server: unexpected error.\n", 1);
	kill(pid, SIGUSR2);
	exit(EXIT_FAILURE);
}

void	get_pid()
{
	ft_putstr_color_fd(ANSI_COLOR_GREEN,
		"PID: ", 1);
	ft_putnbr_fd(getpid(), 1);
	ft_putstr_fd("\n", 1);
}

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
		{
			ft_putstr_fd(message, 1);
			free(message);
			message = 0;
		}
		bits = 0;
		c = 0xFF;
	}
	if (kill(pid, SIGUSR1) == -1)
		error(pid, message);
}

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
	get_pid();
	while (1)
		pause();
}
