/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 20:01:56 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/15 19:06:10 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/execution.h"

void	builtin_exit(char **args, int *exit_status)
{
	printf("exit\n");
	if (args[1])
	{
		*exit_status = ft_atoi(args[1]);
		if (*exit_status == 0 && ft_strcmp(args[1], "0") != 0)
		{
			ft_putendl_fd("minishell: exit: numeric argument required", 2);
			*exit_status = 2;
		}
	}
	exit(*exit_status);
}
