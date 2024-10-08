/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 20:14:16 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/08 18:56:44 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

char	*find_command_path(const char *command, t_env *env)
{
	char	*path;
	char	*path_copy;
	char	*dir;
	char	*full_path;

	path = get_env_value(env, "PATH");
	path_copy = ft_strdup(path);
	dir = ft_strtok(path_copy, ":");
	full_path = NULL;
	while (dir != NULL)
	{
		full_path = malloc(ft_strlen(dir) + ft_strlen(command) + 2);
		ft_strlcpy(full_path, dir, ft_strlen(dir) + 1);
		ft_strlcat(full_path, "/", ft_strlen(full_path) + 2);
		ft_strlcat(full_path, command, ft_strlen(full_path) + ft_strlen(command)
			+ 1);
		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (full_path);
		}
		free(full_path);
		dir = ft_strtok(NULL, ":");
	}
	return (free(path_copy), NULL);
}

void	execute_builtin(t_cmd *cmd, t_env **env)
{
	if (ft_strcmp(cmd->args[0], "cd") == 0)
		builtin_cd(*env, cmd->args);
	else if (ft_strcmp(cmd->args[0], "echo") == 0)
		builtin_echo(cmd->args);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		builtin_env(*env);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		builtin_exit(cmd->args, &(cmd->exit_status));
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		builtin_export(env, cmd->args);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		builtin_pwd();
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		builtin_unset(env, cmd->args);
}

void	execute_command(t_cmd *cmd, t_env **env)
{
	char	*command_path;

	if (is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd, env);
		return ;
	}
	command_path = find_command_path(cmd->args[0], *env);
	if (!command_path)
	{
		ft_putstr_fd("Command not found: ", 2);
		ft_putendl_fd(cmd->args[0], 2);
		return ;
	}
	execve(command_path, cmd->args, NULL);
	perror("execve");
	free(command_path);
}
