/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 20:01:56 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/25 18:12:59 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/execution.h"

void	builtin_exit(char **args, int *exit_status)
{
	int	status;
	int	i;
	int	arg_count;

	arg_count = 0;
	while (args[arg_count])
		arg_count++;
	ft_putendl_fd("exit", 1);
	status = *exit_status;
	if (arg_count > 1)
	{
		// Vérifier si l'argument est numérique
		i = 0;
		while (args[1][i])
		{
			if (i == 0 && (args[1][i] == '-' || args[1][i] == '+'))
				i++;
			if (!ft_isdigit(args[1][i]))
			{
				ft_putstr_fd("minishell: exit: ", 2);
				ft_putstr_fd(args[1], 2);
				ft_putendl_fd(": numeric argument required", 2);
				exit(2);
			}
			i++;
		}
		if (arg_count > 2)
		{
			ft_putendl_fd("minishell: exit: too many arguments", 2);
			return ;
		}
		status = ft_atoi(args[1]);
	}
	exit(status & 255);
}
