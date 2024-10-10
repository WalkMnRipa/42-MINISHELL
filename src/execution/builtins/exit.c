/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 20:01:56 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/10 13:13:13 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/execution.h"

void	builtin_exit(char **args, int *exit_status)
{
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
