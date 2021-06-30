/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlanca-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/28 11:35:11 by mlanca-c          #+#    #+#             */
/*   Updated: 2021/06/30 16:15:47 by mlanca-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_H
# define MINITALK_H

# include <signal.h>
# include "libft.h"

# define STORE 				0
# define PRINT 				1
# define KILL 				2
# define ANSI_COLOR_RED     "\x1b[31m"
# define ANSI_COLOR_GREEN   "\x1b[32m"
# define ANSI_COLOR_YELLOW  "\x1b[33m"
# define ANSI_COLOR_RESET   "\x1b[0m"

/*
** utils.c Functions
*/
char	*init_string(int pid, char *message, char c);
char	*store_string(int pid, char *message, char c);
char	*print_string(int pid, char *message);
char	*kill_string(int pid, char *message);

#endif
