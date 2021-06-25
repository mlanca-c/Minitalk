/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlanca-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/22 12:41:56 by mlanca-c          #+#    #+#             */
/*   Updated: 2021/06/25 12:42:38 by mlanca-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	send_signals(char *message, int pid)
{
	int	i;
	int	shift;

	shift = -1;
	i = 0;
	while (message[i])
	{
		while (++shift < 8)
		{
			if (message[i] & (0x80 >> shift))
			{
				ft_printf("0");
				kill(pid, SIGUSR2);
			}
			else
			{
				ft_printf("1");
				kill(pid, SIGUSR1);
			}
			usleep(3);
		}
		i++;
	}
}

/*
*/
int	main(int argc, char *argv[])
{
	int		pid;

	if (argc != 3 && !ft_str_isnumeric(argv[1]))
	{
		ft_printf("client: invalid arguments\n");
		ft_printf("\tcorrect format [./%s SERVER_PID MESSAGE\n]", argv[0]);
		exit(EXIT_FAILURE);
	}
	pid = ft_atoi(argv[1]);
	send_signals(argv[2], pid);
	return (0);
}
