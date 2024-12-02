/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 10:48:00 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/02 11:44:34 by ggaribot         ###   ########.fr       */
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
	t_heredoc	*last;

	current = cmd->heredocs;
	while (current && !g_signal_received)
	{
		if (pipe(pipe_fds) == -1)
			return (1);
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
			status = write_heredoc(pipe_fds[1], current->delimiter, env,
					current->expand_vars);
			close(pipe_fds[1]);
			exit(status);
		}
		close(pipe_fds[1]);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
		{
			close(pipe_fds[0]);
			return (1);
		}
		// Save the heredoc content to a temporary file
		fd = open(current->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			close(pipe_fds[0]);
			return (1);
		}
		while ((n = read(pipe_fds[0], buffer, sizeof(buffer))) > 0)
			write(fd, buffer, n);
		close(fd);
		close(pipe_fds[0]);
		current = current->next;
	}
	// Set the input file to the last heredoc if no input redirection
	if (cmd->heredocs && !cmd->input_file)
	{
		last = cmd->heredocs;
		while (last->next)
			last = last->next;
		cmd->input_file = ft_strdup(last->filename);
		if (!cmd->input_file)
			return (1);
	}
	return (0);
}
