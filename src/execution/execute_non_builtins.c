/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_non_builtins.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 00:12:22 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/16 15:54:26 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_child_process(t_cmd *cmd, t_env **env)
{
	int	exit_status;

	reset_signals();
	if (is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd, env);
		exit_status = cmd->exit_status;
	}
	else
	{
		execute_external_command(cmd, env);
		exit_status = cmd->exit_status;
	}
	exit(exit_status);
}

static void	handle_parent_process(t_cmd *cmd, pid_t pid)
{
	int	status;
	int	has_exited;

	setup_parent_signals();
	status = 0;
	has_exited = 0;
	while (!has_exited)
	{
		waitpid(pid, &status, WUNTRACED);
		if (WIFEXITED(status) || WIFSIGNALED(status))
			has_exited = 1;
	}
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		ft_putchar_fd('\n', STDERR_FILENO);
	setup_signals();
	update_exit_status(cmd, status);
	if (cmd->exit_status == 127)
		usleep(10);
}

void	execute_non_builtin(t_cmd *cmd, t_env **env)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		handle_child_process(cmd, env);
	else if (pid > 0)
		handle_parent_process(cmd, pid);
	else
	{
		perror("fork");
		cmd->exit_status = 1;
	}
}

int	prepare_command_execution(t_cmd *cmd, t_env **env)
{
	int	status;

	if (cmd->heredocs)
	{
		status = handle_multiple_heredocs(cmd, *env);
		if (status != 0)
		{
			cmd->exit_status = 1;
			return (0);
		}
	}
	if (!setup_redirections(cmd))
	{
		cmd->exit_status = 1;
		return (0);
	}
	return (1);
}
