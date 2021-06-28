/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlanca-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/28 17:58:23 by mlanca-c          #+#    #+#             */
/*   Updated: 2021/06/28 18:33:00 by mlanca-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

/*
*/
void	install_handler(struct sigaction *sa_signal)
{
	sigset_t 			block_mask;

	sigemptyset(&block_mask);
	sigaddset(&block_mask, SIGINT);
	sigaddset(&block_mask, SIGQUIT);
	sa_signal->sa_handler = 0;
	sa_signal->sa_flags = SA_SIGINFO;
	sa_signal->sa_mask = block_mask;
}
