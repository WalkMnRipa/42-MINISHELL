/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 20:14:16 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/02 11:15:18 by ggaribot         ###   ########.fr       */
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

static void	handle_command_error(t_cmd *cmd, char *command_path)
{
	if (!cmd->args[0] || !cmd->args[0][0])
	{
		ft_putendl_fd("minishell: : command not found", 2);
		cmd->exit_status = 127;
		return ;
	}
	if (!command_path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putendl_fd(": command not found", 2);
		cmd->exit_status = 127;
		return ;
	}
}

void	execute_external_command(t_cmd *cmd, t_env **env)
{
	char	*command_path;
	char	**env_array;

	command_path = find_command_path(cmd->args[0], *env);
	handle_command_error(cmd, command_path);
	if (!command_path || cmd->exit_status == 127)
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

static int	prepare_command_execution(t_cmd *cmd, t_env **env)
{
	if (cmd->heredocs && handle_multiple_heredocs(cmd, *env))
		return (0);
	if (!setup_redirections(cmd))
		return (0);
	return (1);
}

void	execute_command(t_cmd *cmd, t_env **env)
{
	int	stdin_backup;
	int	stdout_backup;

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	if (cmd->next)
		execute_pipeline(cmd, env);
	else if (!prepare_command_execution(cmd, env))
		cmd->exit_status = 1;
	else if (is_builtin(cmd->args[0]))
		execute_builtin(cmd, env);
	else
		execute_non_builtin(cmd, env);
	(*env)->last_exit_status = cmd->exit_status;
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
}
