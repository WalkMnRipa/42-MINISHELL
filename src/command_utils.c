/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:38:14 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/18 15:31:29 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

void	execute_single_command(t_cmd *cmd, t_env **env)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	if (is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd, env);
		(*env)->last_exit_status = cmd->exit_status;
	}
	else
		execute_external_command(cmd, env);
}
