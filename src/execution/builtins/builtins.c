/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:08:52 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/18 17:29:15 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/execution.h"

int	is_builtin(char *cmd)
{
	return (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0);
}

void	execute_builtin(t_cmd *cmd, t_env **env)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		builtin_echo(cmd->args);
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		builtin_cd(*env, cmd->args);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		builtin_pwd();
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		builtin_export(env, cmd->args);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		builtin_unset(env, cmd->args);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		builtin_env(*env);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		builtin_exit(cmd->args, &(cmd->exit_status));
}
