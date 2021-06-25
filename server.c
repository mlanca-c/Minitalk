/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlanca-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/22 12:41:13 by mlanca-c          #+#    #+#             */
/*   Updated: 2021/06/25 15:58:30 by mlanca-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

t_message	g_message;

/*
*/
void	handler_sigusr2(int signum)
{
	(void)signum;
	g_message.str[0] |= 0x80 >> g_message.bit;
	g_message.bit++;
	ft_printf("0");
}

/*
*/
void	handler_sigusr1(int signum)
{
	(void)signum;
	g_message.str[0] ^= 0x80 >> g_message.bit;
	g_message.bit++;
	ft_printf("1");
}

/*
*/
int	main(void)
{
	pid_t		pid;

	pid = getpid();
	ft_printf("PID: %d\n", pid);
	g_message.bit = 0;
	g_message.str[SIZE] = '\0';
	ft_memset(g_message.str, 0xFF, SIZE);
	signal(SIGUSR1, handler_sigusr1);
	signal(SIGUSR2, handler_sigusr2);
	// inside while: if done print message
	while (1)
		if (g_message.bit == 8)
		{
			ft_printf("-> %s\n", g_message.str);
			g_message.bit = 0;
			ft_memset(g_message.str, 0xFF, SIZE);
		}
}
