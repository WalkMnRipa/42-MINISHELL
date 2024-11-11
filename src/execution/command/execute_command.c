/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 20:14:16 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/11 19:13:17 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/execution.h"
#include "../../../includes/parsing.h"

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

void	execute_external_command(t_cmd *cmd, t_env **env)
{
	char	*command_path;
	char	**env_array;

	command_path = find_command_path(cmd->args[0], *env);
	if (!command_path)
	{
		handle_command_not_found(cmd);
		return ;
	}
	env_array = env_to_array(*env);
	if (!env_array)
	{
		free(command_path);
		error_exit_message(*env, cmd, ERR_MALLOC_FAILED);
	}
	reset_signals();
	execve(command_path, cmd->args, env_array);
	free_string_array(env_array, -1);
	free(command_path);
	error_exit_message(*env, cmd, "execve failed");
}

static void	execute_non_builtin(t_cmd *cmd, t_env **env)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		execute_external_command(cmd, env);
		exit(cmd->exit_status);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			cmd->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			cmd->exit_status = 128 + WTERMSIG(status);
			if (WTERMSIG(status) == SIGQUIT)
				ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
		}
	}
	else
		cmd->exit_status = (perror("fork"), 1);
}

void	execute_command(t_cmd *cmd, t_env **env)
{
	int	stdin_backup;
	int	stdout_backup;

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	if (cmd->next)
		execute_pipeline(cmd, env);
	else if (!setup_redirections(cmd))
		cmd->exit_status = 1;
	else if (is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd, env);
		(*env)->last_exit_status = cmd->exit_status;
	}
	else
		execute_non_builtin(cmd, env);
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
}
