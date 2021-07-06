/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlanca-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/01 15:45:45 by mlanca-c          #+#    #+#             */
/*   Updated: 2021/07/06 15:55:04 by mlanca-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

/*
** This function is called in case an error occurred in a kill() system call in
** client. It frees 'str' if not null and before exiting the program with
** EXIT_FAILURE it outputs a error message.
*/
void	error(char *str)
{
	if (str)
		free(str);
	ft_putstr_color_fd(ANSI_COLOR_RED,
		"client: unexpected error.\n", 2);
	exit(EXIT_FAILURE);
}

/*
** This function is called by send_bit() when all the bits from message were
** received by server. The send_null() function sends 8 SIGUSR1 signals
** representing the null character in binary - 00000000. Once i equals 8 - once
** all zeros (SIGUSR1) were sent - the function returns 1.
*/
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

/*
** This function sends a bit from the string 'message' everytime ots called.
** It has three static variables:
**
** 	char	*message - It's a duplicated string of the message given in the
** 						main() function of client to send to server. In case
** 						send_bit() function was called with 'str' not being a
** 						null - this only happens in main() - s_pid will be given
** 						the value given in 'pid' parameter. This variable has to
** 						be constant in order for client to not need a global
** 						variable.
** 	int		s_pid	- It's the PID given in the main() function of client. It's
** 						the process ID of server. In case send_bit() function
** 						was called with 'str' not being a null - this only
** 						happens in main() - message will be given the value of
** 						ft_strdup(str). This variable has to be constant in
** 						order for client to be able to send a bit to server
** 						every time send_bit() is called.
** 	int		bits	- It's the bit number from message you want client to send
** 						server. This value is incremented each time send_bit()
** 						function is called.
 *
** In case all the bits from 'message' were already sent to server. send_bit()
** calls the send_null() function. This function sends 8 SIGUSR1 representing
** the null character \0 and signaling the end of the string. Once all 8 bits
** were sent to server 'message' is freed and the function returns 1 telling
** handler_sigusr() that it finished sending the string.
*/
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

/*
** This function is called every time client receives a signal - SIGUSR1 or
** SIGUSR2 from server.
** If the signal received is SIGUSR1 this means that server confirms receiving
** a bit from client and is ready to receive another.
** If the signal received is SIGUSR2 then it means that an error occurred in
** server causing it to quit unexpectedly, then client outputs a error message
** and exits with EXIT_FAILURE.
*/
void	handler_sigusr(int signum)
{
	int	end;

	end = 0;
	if (signum == SIGUSR1)
		end = send_bit(0, 0);
	else if (signum == SIGUSR2)
	{
		ft_putstr_color_fd(ANSI_COLOR_RED,
			"client: server ended unexpectdly.\n", 2);
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
** This is the main function of client.
** @param	int		argc	- argument counter.
** @param	char	**argv	- command line parameters given to client by user.
** 								correct format: [./client <PID> <MESSSAGE>]
**
** @line 2-9		First it checks if the command line parameters given by the
** 					user are correct: 
** 					[./client <PID_SERVER> <STR_TO_SEND_TO_SERVER>]. In case
** 					this parameters are not what client needs to correctly 
** 					function, an error message is displayed and the program
** 					exits with EXIT_FAILURE.
**
** @line 10-11		if the parameters given by the user are correct, the program
** 					continues running and prepares itself to receive either
** 					SIGUSR1 - in case everything is working acordingly in server
** 					or SIGUSR2 - in case of errors from server. The signal()
** 					function is used here with the handler_sigusr() function
** 					being called once these signals are received.
**
** @line 12			after the signal() functions are all set, the program sends
** 					the first bit of 'STR_TO_SEND_TO_SERVER' to server using the 
** 					send_bit() function with the parameters argv[1] - PID, and
** 					argv[2] - message. This is the only time in the program
** 					where send_bit() is called with non null parameters. This
** 					only happens because of its static variables.
** 						**see comments on line 52-65**
**
** @line 13-14		after sending the first bit of the message, and therefore
** 					making the first contact with server, client enters an
** 					infinite loop of pause() functions waiting for either
** 					SIGUSR1 or SIGUSR2. If all goes well and the message is
** 					successfully delivered, client will only receive SIGUSR1
** 					signals from server. These signals are the acknowledgement
** 					from server that it received a bit from client. I chose this
** 					aproach instead of using the sleep() or usleep() functions.
*/
int	main(int argc, char **argv)
{
	if (argc != 3 || !ft_str_isnumeric(argv[1]))
	{
		ft_putstr_color_fd(ANSI_COLOR_RED,
			"client: invalid arguments.\n", 2);
		ft_putstr_color_fd(ANSI_COLOR_YELLOW,
			"correct format: [./client <PID> <STR>].\n", 2);
		exit(EXIT_FAILURE);
	}
	signal(SIGUSR1, handler_sigusr);
	signal(SIGUSR2, handler_sigusr);
	send_bit(ft_atoi(argv[1]), argv[2]);
	while (1)
		pause();
}
