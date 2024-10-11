/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 20:14:16 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/11 15:41:57 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

char	*find_command_path(const char *command, t_env *env)
{
	char	*path;
	char	*path_copy;
	char	*dir;
	char	*result;

	if (is_direct_executable(command))
		return (ft_strdup(command));
	path = get_env_value(env, "PATH");
	if (!path)
		return (NULL);
	path_copy = ft_strdup(path);
	if (!path_copy)
		return (NULL);
	dir = ft_strtok(path_copy, ":");
	while (dir != NULL)
	{
		result = try_path(dir, command);
		if (result)
		{
			free(path_copy);
			return (result);
		}
		dir = ft_strtok(NULL, ":");
	}
	return (free(path_copy), NULL);
}

static void	handle_command_not_found(t_cmd *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd->args[0], 2);
	ft_putendl_fd(": command not found", 2);
	cmd->exit_status = 127;
}

static void	execute_child_process(char *command_path, t_cmd *cmd)
{
	execve(command_path, cmd->args, NULL);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd->args[0], 2);
	ft_putstr_fd(": ", 2);
	perror("");
	exit(126);
}

static void	wait_for_child(pid_t pid, t_cmd *cmd)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		cmd->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		cmd->exit_status = 128 + WTERMSIG(status);
}

void	execute_command(t_cmd *cmd, t_env **env)
{
	char	*command_path;
	pid_t	pid;

	if (is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd, env);
		return ;
	}
	command_path = find_command_path(cmd->args[0], *env);
	if (!command_path)
	{
		handle_command_not_found(cmd);
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		cmd->exit_status = 1;
	}
	else if (pid == 0)
		execute_child_process(command_path, cmd);
	else
		wait_for_child(pid, cmd);
	free(command_path);
}
