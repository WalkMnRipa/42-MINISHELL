/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_non_builtins.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 00:12:22 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/17 17:20:14 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_child_process(t_cmd *cmd, t_env **env)
{
	reset_signals();
	execute_external_command(cmd, env);
	cleanup_all(*env, cmd, cmd->exit_status);
}

static void	handle_non_builtin_parent(t_cmd *cmd, pid_t pid)
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
		handle_non_builtin_parent(cmd, pid);
	else
	{
		perror("fork");
		cmd->exit_status = 1;
	}
}

int	prepare_command_execution(t_cmd *cmd, t_env **env)
{
	if (cmd->heredocs)
	{
		if (handle_multiple_heredocs(cmd, *env) != 0)
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
