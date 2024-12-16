/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 10:48:00 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/16 18:46:43 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	cleanup_heredoc_resources(t_heredoc *current)
{
	if (current && current->filename)
	{
		unlink(current->filename);
		free(current->filename);
		current->filename = NULL;
	}
}

void	run_heredoc_child(t_heredoc *current, t_env *env, int pipe_fds[2])
{
	close(pipe_fds[0]);
	signal(SIGINT, handle_heredoc_signal);
	write_heredoc(pipe_fds[1], current->delimiter, env, current->expand_vars);
	close(pipe_fds[1]);
	exit(0);
}

static int	write_pipe_content(int fd, int pipe_fd, t_heredoc *current)
{
	char	buffer[4096];
	ssize_t	n;

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

int	copy_pipe_to_file(int pipe_fd, t_heredoc *current)
{
	int	fd;

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
	return (write_pipe_content(fd, pipe_fd, current));
}
