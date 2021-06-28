/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlanca-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/28 15:34:06 by mlanca-c          #+#    #+#             */
/*   Updated: 2021/06/28 19:47:57 by mlanca-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	add_character(int pid, char c)
{
	static char	*message;
	char		*temporary;
	int			i;
	int			size;

	if (!message)
	{
		message = (char *)malloc(sizeof(char) * 2);
		message[0] = c;
		message[1] = '\0';
	}
	if (!c)
	{
		ft_putstr_fd(message, 1);
		free(message);
		kill(pid, SIGUSR1);
	}
	temporary = message;
	size = ft_strlen(message);
	message = (char *)malloc(sizeof(char) * (size + 2));
	i = -1;
	while (temporary[++i])
		message[i] = temporary[i];
	message[size + 1] = '\0';
	message[size] = c;
	if (temporary)
		free(temporary);
	printf("add: %s\n", message);
}

void	handler_sigusr(int signum, siginfo_t *info, void *context)
{
	char		c;
	static int	bits = 0;

	(void)context;
	printf("bits: %d\n", bits);
	if (bits % 8 == 0)
		c = 0xFF;
	if (signum == SIGUSR1)
		c ^= 0x80 >> bits % 8;
	else if (signum == SIGUSR2)
		c |= 0x80 >> bits % 8;
	if (kill(info->si_pid, SIGUSR1) == -1)
	{
		kill(info->si_pid, SIGUSR2);
		exit(1);
	}
	bits++;
	if (bits % 8 == 0)
		add_character((int)info->si_pid, c);
}

int	main(void)
{
	struct sigaction	sa_signal;

	ft_putstr_fd("PID: ", 1);
	ft_putnbr_fd(getpid(), 1);
	ft_putstr_fd("\n", 1);
	install_handler(&sa_signal);
	sa_signal.sa_sigaction = handler_sigusr;
	sigaction(SIGUSR1, &sa_signal, NULL);
	sigaction(SIGUSR2, &sa_signal, NULL);
	while (1)
		;
}
