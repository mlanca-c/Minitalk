# Minitalk
 
 [![Intro](https://img.shields.io/badge/Cursus-Minitalk-success?style=for-the-badge&logo=42)](https://github.com/mlanca-c/Minitalk)
 
 [![Stars](https://img.shields.io/github/stars/mlanca-c/Minitalk?color=ffff00&label=Stars&logo=Stars&style=?style=flat)](https://github.com/mlanca-c/Minitalk)
 [![Size](https://img.shields.io/github/repo-size/mlanca-c/Minitalk?color=blue&label=Size&logo=Size&style=?style=flat)](https://github.com/mlanca-c/Minitalk)
 [![Activity](https://img.shields.io/github/last-commit/mlanca-c/Minitalk?color=red&label=Last%20Commit&style=flat)](https://github.com/mlanca-c/Minitalk)

# Cloning

 ```
 git clone --recursive git@github.com:mlanca-c/Minitalk.git
 cd Minitalk
 make all
 ```
 
 # Compiling
 
 ```
 $ ./server
 PID: <PID_SERVER>
 
 (wait)
 ```

 ```
 $ ./client <PID_SERVER> <STRING>
 ```

# Intro
 > [subject](subject.pdf)

# Useful Links

 * [Wiki](https://github.com/mlanca-c/Minitalk/wiki)
 * [Text Generator](http://www.dummytextgenerator.com/#jump)


```c
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoinc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlanca-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/01 17:32:39 by mlanca-c          #+#    #+#             */
/*   Updated: 2021/07/01 18:16:08 by mlanca-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
*/
char	*ft_straddc_first(char c)
{
	char	*add;

	add = (char *)malloc(sizeof(char) * 2);
	if (!add)
		return (NULL);
	add[0] = c;
	add[1] = '\0';
	return (add);
}

/*
*/
char	*ft_straddc(char *str, char c)
{
	char	*add;
	int		i;

	if (!c)
		return (NULL);
	if (!str)
		return (ft_straddc_first(c));
	add = (char *)malloc(sizeof(char) * (ft_strlen(str) + 2));
	if (!add)
	{
		free(str);
		return (NULL);
	}
	i = -1;
	while (str[++i])
		add[i] = str[i];
	free(str);
	add[i++] = c;
	add[i] = '\0';
	return (add);
}
```
