/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils5.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 18:43:24 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/16 18:49:49 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_parent_process(pid_t pid, t_heredoc *current,
		int pipe_fds[2], int *has_error)
{
	int	status;

	close(pipe_fds[1]);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		close(pipe_fds[0]);
		cleanup_heredoc_resources(current);
		setup_signals();
		*has_error = 1;
		return (1);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		close(pipe_fds[0]);
		cleanup_heredoc_resources(current);
		setup_signals();
		*has_error = 1;
		return (1);
	}
	if (copy_pipe_to_file(pipe_fds[0], current))
		return (*has_error = 1, 1);
	return (0);
}

static int	process_heredoc(t_heredoc *current, t_env *env, int *has_error)
{
	int		pipe_fds[2];
	pid_t	pid;

	if (pipe(pipe_fds) == -1)
	{
		*has_error = 1;
		return (1);
	}
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
	{
		close(pipe_fds[0]);
		close(pipe_fds[1]);
		*has_error = 1;
		return (1);
	}
	if (pid == 0)
		run_heredoc_child(current, env, pipe_fds);
	return (handle_parent_process(pid, current, pipe_fds, has_error));
}

static int	process_command_heredocs(t_cmd *current_cmd,
		t_heredoc *current_heredoc, t_env *env, int *has_error)
{
	while (current_heredoc && !*has_error)
	{
		if (process_heredoc(current_heredoc, env, has_error))
		{
			cleanup_heredoc_resources(current_heredoc);
			g_signal_received = 1;
			setup_signals();
			return (1);
		}
		current_heredoc = current_heredoc->next;
	}
	if (setup_last_heredoc(current_cmd))
		return (1);
	return (0);
}

int	handle_multiple_heredocs(t_cmd *cmd, t_env *env)
{
	t_cmd	*current_cmd;
	int		has_error;

	has_error = 0;
	current_cmd = cmd;
	while (current_cmd && !has_error)
	{
		if (current_cmd->heredocs)
		{
			if (process_command_heredocs(current_cmd, current_cmd->heredocs,
					env, &has_error))
				return (1);
		}
		current_cmd = current_cmd->next;
	}
	setup_signals();
	return (0);
}
