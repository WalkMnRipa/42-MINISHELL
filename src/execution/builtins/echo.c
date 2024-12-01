/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 19:57:03 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/01 23:15:21 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	write_with_check(int fd, const void *buf, size_t count)
{
	ssize_t	result;

	result = write(fd, buf, count);
	return (result == -1);
}

static int	is_valid_n_flag(const char *str)
{
	int	i;

	if (!str || str[0] != '-' || str[1] != 'n')
		return (0);
	i = 2;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_echo(char **args)
{
	int	i;
	int	n_flag;

	if (!args)
		return (1);
	n_flag = 0;
	i = 1;
	while (args[i] && is_valid_n_flag(args[i]))
	{
		n_flag = 1;
		i++;
	}
	while (args[i])
	{
		if (write_with_check(STDOUT_FILENO, args[i], ft_strlen(args[i])))
			return (1);
		if (args[i + 1])
			if (write_with_check(STDOUT_FILENO, " ", 1))
				return (1);
		i++;
	}
	if (!n_flag)
		if (write_with_check(STDOUT_FILENO, "\n", 1))
			return (1);
	return (0);
}
