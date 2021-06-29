/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlanca-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/28 11:34:11 by mlanca-c          #+#    #+#             */
/*   Updated: 2021/06/29 20:00:41 by mlanca-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

/*
** Global variable f the project. This string is a duplicate - ft_strdup() - of
** argv[2] the string 'client' wants to send to 'server'. This string needs to
** global so that send_bit() function can get to it once send_bit() can only be
** accessed through handler_sigusr that is of type
** (void       (*sa_sigaction)(int, siginfo_t *, void *)).
*/
char	*g_message;

/*
*/
void	error(void)
{
	free(g_message);
	exit(EXIT_FAILURE);
}

/*
 * This function extracts a bit from a character from a string.
 * It has two static variables in order to iterate the string given as a
 * command line argument bit-by-bit.
 * The first static - 'int i' iterates the string - 'g_message' -
 * character-by-character. It starts at zero and it ends at length - 1.
 * The other static - 'int bit' iterates a character - 'g_message[i]' -
 * bit-by-bit. It starts at zero and it ends at 7 (8 bits per character from 0
 * to 7) for each character of g_message.
 * Since client waits to receive 'SIGUSR1' before sending the next bit, both
 * variables need to be static so that when send_bit() function is called again
 * by handler_sigusr, it knows which bit to send next.
 * Once all of g_message was iterated. 8 SIGUSR1 signals are sent to server.
 * That represents 8 zeros - 00000000 - the NULL character. g_message is freed.
 *
 * @param	pid	- Process ID of server (process where client sends the signals
 * 										- bits - to).
*/
void	send_bit(int pid)
{
	static int	i = 0;
	static int	bit = 0;

	printf("send: %s\n", &g_message[i]);
	if (g_message[i])
	{
		if (g_message[i] & (0x80 >> bit))
		{
			printf("1");
			if (kill(pid, SIGUSR2) == -1)
				error();
		}
		else
		{
			printf("0");
			if (kill(pid, SIGUSR1) == -1)
				error();
		}
		if (++bit == 8)
		{
			printf("\n");
			bit = 0;
			i++;
		}
	}
	else
	{
		free(g_message);
		while (++bit < 8)
			if (kill(pid, SIGUSR1) == -1)
				exit(EXIT_FAILURE);
	}
}

/*
 * This function is a handler function for when client receives SIGUSR1 or
 * SIGUSR2.
 * SIGUSR1:	is sent to client as a request for more bits from g_message.
 * SIGUSR2:	is sent to client as a confirmation that all bits from g_message
 * 			were successfully sent to server.
 *
 * If the handler_sigusr() function is handling the SIGUSR1 signal, then
 * send_bit() function is called and the next bit of the string - g_message - is
 * sent to server.
 * If the handler_sigusr() function is handling the SIGUSR2 signal, then
 * a success message is displayed and the program ends.
 *
 * @param	int			signum		- signal client received from server. It can
 * 									either e SIGUSR1 or SIGUSR2.
 * @param	siginfo_t	*info		- struct that contains info about the source
 * 									 of the signal - info->si_pid: PID of the
 * 									 process that sent the signal - server.
 * @param	void		*context	- points to a ucontext_t containing the
 * 									context information at the time of the
 * 									signal interrupt - not used.
*/
void	handler_sigusr(int signum, siginfo_t *info, void *context)
{
	(void)context;
	if (signum == SIGUSR1)
		send_bit(info->si_pid);
	else if (signum == SIGUSR2)
	{
		ft_putstr_fd("server: operation successful.\n", 1);
		exit(EXIT_SUCCESS);
	}
}

/*
 * This is the main function of client. It basically runs in a infinite loop
 * while waiting for updates from server:
 * 		SIGUSR1 from server - send next bit.
 * 		SIGUSR2 from server - all bits were received successfully.
 *
 * @line 122-123	- g_message becomes a duplicate of the <STR> sent as a
 * 					command line parameter. g_message is a global variable
 * 					therefore it is within the reach of every function inside of
 * 					the client.c file.
 *
 * @line 122-123	- then client sends the first bit to server -
 * 					send_bit() function. This will create the first contact
 * 					between the two processes. Enabling both to contact each
 * 					other from now on.
 *
 * @line 122-123	- The only reason both processes can send signals to each
 * 					other is because instead of signal() the programs both
 * 					use sigaction(), making the handler functions of type
 * 					sa_sigaction.
 *
 * @line 122-123	- The program then enters an infinite while loop until it
 * 					receives SIGUSR2.
 *
 * @param	int		argc	- command line argument counter.
 *
 * @param	char	**argv	- ./client <SERVER_PID> <STR> - command line 
 * 							arguments.
*/
int	main(int argc, char **argv)
{
	struct sigaction	sa_signal;
	sigset_t 			block_mask;

	if (argc != 3)
	{
		ft_putstr_fd("client: invalid arguments.\n", 1);
		ft_putstr_fd("correct format: [./client <PID> <STR>].\n", 1);
		exit(EXIT_FAILURE);
	}
	g_message = ft_strdup(argv[2]);
	if (!g_message)
		exit(EXIT_FAILURE);
	send_bit(ft_atoi(argv[1]));
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
