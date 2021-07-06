/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlanca-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/01 15:45:45 by mlanca-c          #+#    #+#             */
/*   Updated: 2021/07/06 13:05:06 by mlanca-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	error(char *str)
{
	if (str)
		free(str);
	ft_putstr_color_fd(ANSI_COLOR_RED,
		"client: unexpected error.\n", 1);
	exit(EXIT_FAILURE);
}

int	send_null(int pid, char *str)
{
	static int	i = 0;

	if (i++ != 8)
	{
		if (kill(pid, SIGUSR1) == -1)
			error(str);
		return (0);
	}
	return (1);
}

int	send_bit(int pid, char *str)
{
	static char	*message = 0;
	static int	s_pid = 0;
	static int	bits = -1;

	if (str)
		message = ft_strdup(str);
	if (!message)
		error(0);
	if (pid)
		s_pid = pid;
	if (message[++bits / 8])
	{
		if (message[bits / 8] & (0x80 >> (bits % 8)))
		{
			if (kill(s_pid, SIGUSR2) == -1)
				error(message);
		}
		else if (kill(s_pid, SIGUSR1) == -1)
			error(message);
		return (0);
	}
	if (!send_null(s_pid, message))
		return (0);
	free(message);
	return (1);
}

void	handler_sigusr(int signum)
{
	int	end;

	end = 0;
	if (signum == SIGUSR1)
		end = send_bit(0, 0);
	else if (signum == SIGUSR2)
	{
		ft_putstr_color_fd(ANSI_COLOR_RED,
			"client: server ended unexpectdly.\n", 1);
		exit(EXIT_FAILURE);
	}
	if (end)
	{
		ft_putstr_color_fd(ANSI_COLOR_GREEN,
			"client: operation successful.\n", 1);
		exit(EXIT_SUCCESS);
	}
}

/*
 * The main funcion will send a bit and then wait for a signal.
*/
int	main(int argc, char **argv)
{
	if (argc != 3 || !ft_str_isnumeric(argv[1]))
	{
		ft_putstr_color_fd(ANSI_COLOR_RED,
			"client: invalid arguments.\n", 1);
		ft_putstr_color_fd(ANSI_COLOR_YELLOW,
			"correct format: [./client <PID> <STR>].\n", 1);
		exit(EXIT_FAILURE);
	}
	signal(SIGUSR1, handler_sigusr);
	signal(SIGUSR2, handler_sigusr);
	send_bit(ft_atoi(argv[1]), argv[2]);
	while (1)
		pause();
}
