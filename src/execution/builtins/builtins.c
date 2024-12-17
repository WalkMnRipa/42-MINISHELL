/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:08:52 by jcohen            #+#    #+#             */
/*   Updated: 2024/12/17 17:25:03 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

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
		cmd->exit_status = builtin_echo(cmd->args);
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		cmd->exit_status = builtin_cd(cmd, *env, cmd->args);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		cmd->exit_status = builtin_pwd();
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		cmd->exit_status = builtin_export(cmd, env, cmd->args);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		cmd->exit_status = builtin_unset(cmd, env, cmd->args);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		cmd->exit_status = builtin_env(*env);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		cmd->exit_status = builtin_exit(cmd, env, cmd->args);
	(*env)->last_exit_status = cmd->exit_status;
}
