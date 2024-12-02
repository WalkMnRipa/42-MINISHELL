/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 10:48:00 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/02 14:01:40 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
static t_heredoc	*get_last_heredoc(t_cmd *cmd)
{
	t_heredoc	*current;

	current = cmd->heredocs;
	while (current && current->next)
		current = current->next;
	return (current);
}

static int	set_input_from_last_heredoc(t_cmd *cmd)
{
	t_heredoc	*last;

	if (!cmd->input_file)
	{
		last = get_last_heredoc(cmd);
		if (last)
		{
			cmd->input_file = ft_strdup(last->filename);
			if (!cmd->input_file)
				return (1);
		}
	}
	return (0);
}
*/

int	handle_multiple_heredocs(t_cmd *cmd, t_env *env)
{
	t_heredoc	*current;
	int			pipe_fds[2];
	pid_t		pid;
	int			status;
	int			fd;
	char		buffer[4096];
	ssize_t		n;

	current = cmd->heredocs;
	while (current)
	{
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
		{
			close(pipe_fds[0]);
			signal(SIGINT, handle_heredoc_signal);
			status = write_heredoc(pipe_fds[1], current->delimiter, env,
					current->expand_vars);
			close(pipe_fds[1]);
			exit(status);
		}
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
		fd = open(current->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			close(pipe_fds[0]);
			return (1);
		}
		while ((n = read(pipe_fds[0], buffer, sizeof(buffer))) > 0)
		{
			if (write(fd, buffer, n) != n)
			{
				close(fd);
				close(pipe_fds[0]);
				return (1);
			}
		}
		close(fd);
		close(pipe_fds[0]);
		current = current->next;
	}
	setup_signals();
	if (cmd->heredocs && !cmd->input_file)
	{
		current = cmd->heredocs;
		while (current->next)
			current = current->next;
		cmd->input_file = ft_strdup(current->filename);
		if (!cmd->input_file)
			return (1);
	}
	return (0);
}
