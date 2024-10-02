/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 20:01:56 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/01 20:02:01 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/execution.h"

void	builtin_exit(t_shell *shell, char **args)
{
	int	exit_status;

	ft_putendl_fd("exit", 1);
	if (args[1])
	{
		exit_status = ft_atoi(args[1]);
		if (exit_status == 0 && ft_strcmp(args[1], "0") != 0)
		{
			ft_putendl_fd("minishell: exit: numeric argument required", 2);
			exit_status = 2;
		}
	}
	else
		exit_status = shell->exit_status;
	cleanup_shell(shell);
	exit(exit_status);
}
