/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 20:01:56 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/18 17:29:12 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/execution.h"

void	builtin_exit(char **args, int *exit_status)
{
	int	status;

	status = *exit_status;
	printf("exit\n");
	if (args[1])
	{
		status = ft_atoi(args[1]);
		if (status == 0 && ft_strcmp(args[1], "0") != 0)
		{
			ft_putendl_fd("minishell: exit: numeric argument required", 2);
			status = 2;
		}
	}
	exit(status);
}
