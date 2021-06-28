/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlanca-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/28 11:34:11 by mlanca-c          #+#    #+#             */
/*   Updated: 2021/06/28 19:47:58 by mlanca-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

char	*g_message;

/*
 * This function sends character, once called, of g_message to server. Everytime
 * send_character() function is called the next character of g_message is sent
 * via signals - SIGUSR1 and SIGUSR2 - to the process with the PID 'pid'.
 *
 * @param	pid	- Process ID of server (process where client sends the signals
 * 										to).
*/
int	send_character(int pid)
{
	static int	i = 0;
	int			shift;

	shift = 0;
	if (g_message[i - 1] || g_message[i])
	{
		printf("send_character: %c\n", g_message[i]);
		while (shift < 8)
		{
			if (g_message[i] & 0x80 >> shift)
			{
				if (kill(pid, SIGUSR2) == -1)
					exit(EXIT_FAILURE);
			}
			else
				if (kill(pid, SIGUSR1) == -1)
					exit(EXIT_FAILURE);
			shift++;
		}
		shift = 0;
		i++;
	}
	if (!g_message[i])
	{
		free(g_message);
		return (1);
	}
	return (0);
}

/*
*/
void	handler_sigusr(int signum, siginfo_t *info, void *context)
{
	(void)context;
	if (signum == SIGUSR1)
	{
		if (send_character(info->si_pid))
			pause();
	}
	else
	{
		ft_putstr_fd("An error occured: server did not receive message.\n", 1);
		exit(1);
	}
}

/*
** This is the main function of client.
** In here the program checks if the parameters sent in the compilation are 
** correct - only two parameters can be sent, the PID address of server and the
** string to be sent.
** The program then calls the send_character() function - sends the first 8
** signals of the program. And then it waits for a signal - SIGUSR1 or SIGUSR2
** from server.
** If client receives SIGUSR1 - then the next character of the string is sent to
** server. If the client recieves SIGUSR2 - then a error occurd or all the
** characters of the string were already sent.
**
** @param	int		argc	- argument counter.
**
** @param	char	**argv	- ./client <SERVER_PID> <STR> - parameters.
*/
int	main(int argc, char **argv)
{
	struct sigaction	sa_signal;

	if (argc != 3 || !ft_str_isnumeric(argv[1]))
	{
		ft_putstr_fd("client: invalid arguments.\n", 1);
		ft_putstr_fd("\tcorrect format: ./client <server_pid> <message>\n", 1);
		exit(EXIT_FAILURE);
	}
	g_message = ft_strdup(argv[2]);
	printf("g_message: %s\n", g_message);
	send_character(ft_atoi(argv[1]));
	install_handler(&sa_signal);
	sa_signal.sa_sigaction = handler_sigusr;
	sigaction(SIGUSR1, &sa_signal, NULL);
	sigaction(SIGUSR2, &sa_signal, NULL);
	while (1)
		;
	return (0);
}
