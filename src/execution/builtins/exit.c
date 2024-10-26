/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 20:01:56 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/26 15:24:36 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/execution.h"

void	builtin_exit(t_cmd *cmd, char **args)
{
	int	status;

	status = cmd->exit_status;
	printf("exit\n");
	if (args[1])
	{
		status = ft_atoi(args[1]);
		if (status == 0 && ft_strcmp(args[1], "0") != 0)
		{
			ft_putendl_fd("minishell: exit: numeric argument required", 2);
			exit(2);
		}
		if (args[2])
		{
			ft_putendl_fd("minishell: exit: too many arguments", 2);
			cmd->exit_status = 1;
			return ;
		}
	}
	exit(status);
}
