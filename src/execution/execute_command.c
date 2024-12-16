/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 20:14:16 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/16 15:53:21 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
			return (free(path_copy), result);
		dir = ft_strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}

static int	handle_command_error(t_cmd *cmd, char *command_path, t_env **env)
{
	if (!cmd->args[0] || !cmd->args[0][0])
	{
		ft_putendl_fd("minishell: : command not found", 2);
		cmd->exit_status = 127;
		if (command_path)
			free(command_path);
		cleanup(*env, cmd);
		exit(127);
		return (1);
	}
	if (!command_path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putendl_fd(": command not found", 2);
		cmd->exit_status = 127;
		cleanup(*env, cmd);
		exit(127);
		return (1);
	}
	return (0);
}

void	execute_external_command(t_cmd *cmd, t_env **env)
{
	char	*command_path;
	char	**env_array;

	command_path = find_command_path(cmd->args[0], *env);
	if (handle_command_error(cmd, command_path, env))
		return ;
	env_array = env_to_array(*env);
	if (!env_array)
	{
		free(command_path);
		error_exit_message(*env, cmd, "malloc failed");
	}
	reset_signals();
	execve(command_path, cmd->args, env_array);
	perror("execve failed");
	free(command_path);
	free_string_array(env_array, -1);
	exit(1);
}

static void	execute_command_with_fds(t_cmd *cmd, t_env **env)
{
	if (cmd->next)
		execute_pipeline(cmd, env);
	else if (is_builtin(cmd->args[0]))
		execute_builtin(cmd, env);
	else
		execute_non_builtin(cmd, env);
	cleanup_heredoc_files(cmd);
	(*env)->last_exit_status = cmd->exit_status;
}

void	execute_command(t_cmd *cmd, t_env **env)
{
	int	stdin_backup;
	int	stdout_backup;

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	if (!cmd->next && !prepare_command_execution(cmd, env))
	{
		dup2(stdin_backup, STDIN_FILENO);
		dup2(stdout_backup, STDOUT_FILENO);
		close(stdin_backup);
		close(stdout_backup);
		return ;
	}
	execute_command_with_fds(cmd, env);
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
}
