/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 10:48:00 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/02 15:18:36 by ggaribot         ###   ########.fr       */
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
		return (1);
	}
	n = read(pipe_fd, buffer, sizeof(buffer));
	while (n > 0)
	{
		if (write(fd, buffer, n) != n)
		{
			close(fd);
			close(pipe_fd);
			return (1);
		}
		n = read(pipe_fd, buffer, sizeof(buffer));
	}
	close(fd);
	close(pipe_fd);
	return (0);
}

static int	handle_parent_process(pid_t pid, t_heredoc *current,
		int pipe_fds[2])
{
	int	status;

	close(pipe_fds[1]);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		status = WEXITSTATUS(status);
		if (status != 0)
		{
			close(pipe_fds[0]);
			setup_signals();
			return (status);
		}
	}
	return (copy_pipe_to_file(pipe_fds[0], current));
}

static int	run_heredoc_child(t_heredoc *current, t_env *env, int pipe_fds[2])
{
	int	status;

	close(pipe_fds[0]);
	signal(SIGINT, handle_heredoc_signal);
	status = write_heredoc(pipe_fds[1], current->delimiter, env,
			current->expand_vars);
	close(pipe_fds[1]);
	exit(status);
}

static int	process_heredoc(t_heredoc *current, t_env *env)
{
	int		pipe_fds[2];
	pid_t	pid;
	int		ret;

	if (pipe(pipe_fds) == -1)
		return (1);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
	{
		close(pipe_fds[0]);
		close(pipe_fds[1]);
		return (1);
	}
	if (pid == 0)
		run_heredoc_child(current, env, pipe_fds);
	ret = handle_parent_process(pid, current, pipe_fds);
	return (ret);
}

int	handle_multiple_heredocs(t_cmd *cmd, t_env *env)
{
	t_heredoc	*current;
	int			status;

	current = cmd->heredocs;
	while (current)
	{
		status = process_heredoc(current, env);
		if (status != 0)
			return (status);
		current = current->next;
	}
	setup_signals();
	return (setup_last_heredoc(cmd));
}