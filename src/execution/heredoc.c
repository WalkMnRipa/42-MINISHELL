/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:38:41 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/25 16:57:22 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

static void	write_heredoc(int tmp_fd, char *delimiter)
{
	char	*line;
	size_t	delim_len;

	delim_len = ft_strlen(delimiter);
	while (1)
	{
		ft_putstr_fd("> ", STDERR_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		if (ft_strncmp(line, delimiter, delim_len) == 0
			&& line[delim_len] == '\n' && ft_strlen(line) == delim_len + 1)
		{
			free(line);
			break ;
		}
		write(tmp_fd, line, ft_strlen(line));
		free(line);
	}
}

void	handle_heredoc(t_cmd *cmd, char *delimiter)
{
	int	pipe_fd[2];

	if (!cmd || !delimiter)
		return ;
	if (pipe(pipe_fd) == -1)
	{
		perror("minishell: pipe");
		cmd->exit_status = 1;
		return ;
	}
	write_heredoc(pipe_fd[1], delimiter);
	close(pipe_fd[1]);
	if (cmd->input_fd != STDIN_FILENO)
		close(cmd->input_fd);
	cmd->input_fd = pipe_fd[0];
	if (cmd->input_file)
	{
		free(cmd->input_file);
		cmd->input_file = NULL;
	}
}
