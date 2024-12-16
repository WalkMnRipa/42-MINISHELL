/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 10:48:00 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/16 18:29:09 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	copy_pipe_to_file(int pipe_fd, t_heredoc *current)
{
	int		fd;
	char	buffer[4096];
	ssize_t	n;

	fd = open(current->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		close(pipe_fd);
		if (current->filename)
		{
			unlink(current->filename);
			free(current->filename);
			current->filename = NULL;
		}
		return (1);
	}
	n = read(pipe_fd, buffer, sizeof(buffer));
	while (n > 0)
	{
		if (write(fd, buffer, n) != n)
		{
			close(fd);
			close(pipe_fd);
			unlink(current->filename);
			free(current->filename);
			current->filename = NULL;
			return (1);
		}
		n = read(pipe_fd, buffer, sizeof(buffer));
	}
	close(fd);
	close(pipe_fd);
	return (0);
}

static void	cleanup_heredoc_resources(t_heredoc *current)
{
	if (current && current->filename)
	{
		unlink(current->filename);
		free(current->filename);
		current->filename = NULL;
	}
}

static void	run_heredoc_child(t_heredoc *current, t_env *env, int pipe_fds[2])
{
	close(pipe_fds[0]);
	signal(SIGINT, handle_heredoc_signal);
	write_heredoc(pipe_fds[1], current->delimiter, env, current->expand_vars);
	close(pipe_fds[1]);
	exit(0);
}

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
	if (WIFEXITED(status))
	{
		status = WEXITSTATUS(status);
		if (status != 0)
		{
			close(pipe_fds[0]);
			cleanup_heredoc_resources(current);
			setup_signals();
			*has_error = 1;
			return (1);
		}
	}
	if (copy_pipe_to_file(pipe_fds[0], current))
	{
		*has_error = 1;
		return (1);
	}
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

static void	cleanup_on_error(t_cmd *cmd)
{
	t_cmd		*current_cmd;
	t_heredoc	*current_heredoc;

	current_cmd = cmd;
	while (current_cmd)
	{
		if (current_cmd->heredocs)
		{
			current_heredoc = current_cmd->heredocs;
			while (current_heredoc)
			{
				cleanup_heredoc_resources(current_heredoc);
				current_heredoc = current_heredoc->next;
			}
		}
		current_cmd = current_cmd->next;
	}
}

int	handle_multiple_heredocs(t_cmd *cmd, t_env *env)
{
	t_cmd		*current_cmd;
	t_heredoc	*current_heredoc;
	int			has_error;

	has_error = 0;
	current_cmd = cmd;
	while (current_cmd && !has_error)
	{
		if (current_cmd->heredocs)
		{
			current_heredoc = current_cmd->heredocs;
			while (current_heredoc && !has_error)
			{
				if (process_heredoc(current_heredoc, env, &has_error))
				{
					cleanup_on_error(cmd);
					g_signal_received = 1;
					setup_signals();
					return (1);
				}
				current_heredoc = current_heredoc->next;
			}
			if (setup_last_heredoc(current_cmd))
			{
				cleanup_on_error(cmd);
				return (1);
			}
		}
		current_cmd = current_cmd->next;
	}
	setup_signals();
	return (0);
}
