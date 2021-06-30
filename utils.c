/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlanca-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/30 15:27:03 by mlanca-c          #+#    #+#             */
/*   Updated: 2021/06/30 16:02:40 by mlanca-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

char	*init_string(int pid, char *message, char c)
{
	message = (char *)malloc(sizeof(char) * 2);
	if (!message)
		exit(EXIT_FAILURE);
	message[0] = c;
	message[1] = '\0';
	kill_string(pid, message);
	return (message);
}

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

char	*print_string(int pid, char *message)
{
	ft_putstr_fd(message, 1);
	message = 0;
	free(message);
	if (kill(pid, SIGUSR2) == -1)
		exit(EXIT_FAILURE);
	return (NULL);
}

char	*kill_string(int pid, char *message)
{
	if (kill(pid, SIGUSR1) == -1)
	{
		message = 0;
		free(message);
		exit(EXIT_FAILURE);
	}
	return (message);
}
