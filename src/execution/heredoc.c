/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:38:41 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/25 16:46:00 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

static void	cleanup_heredoc(void)
{
	unlink(HEREDOC_TMP);
}

static void	write_heredoc(int tmp_fd, char *delimiter)
{
	char	*line;
	size_t	delim_len;

	delim_len = ft_strlen(delimiter);
	while (1)
	{
		ft_putstr_fd(HEREDOC_PROMPT, 1);
		line = get_next_line(0);
		if (!line)
			break ;
		if (ft_strncmp(line, delimiter, delim_len) == 0
			&& line[delim_len] == '\n' && ft_strlen(line) == delim_len + 1)
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, tmp_fd);
		free(line);
	}
}

void	handle_heredoc(t_cmd *cmd, char *delimiter)
{
	int	tmp_fd;

	if (!cmd || !delimiter)
		return ;
	cleanup_heredoc();
	tmp_fd = open(HEREDOC_TMP, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (tmp_fd < 0)
	{
		ft_putendl_fd(ERR_HEREDOC_CREATING, 2);
		cmd->exit_status = 1;
		return ;
	}
	write_heredoc(tmp_fd, delimiter);
	close(tmp_fd);
	cmd->input_fd = open(HEREDOC_TMP, O_RDONLY);
	if (cmd->input_fd < 0)
	{
		ft_putendl_fd(ERR_HEREDOC_OPEN, 2);
		cleanup_heredoc();
		cmd->exit_status = 1;
		return ;
	}
	if (cmd->input_file)
		free(cmd->input_file);
	cmd->input_file = ft_strdup(HEREDOC_TMP);
}
