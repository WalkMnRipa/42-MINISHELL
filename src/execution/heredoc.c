/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:38:41 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/07 17:04:30 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

static void	write_heredoc(int tmp_fd, char *delimiter)
{
	char	*line;

	while (1)
	{
		ft_putstr_fd("> ", 1);
		line = get_next_line(0);
		if (!line)
			break ;
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
			&& line[ft_strlen(delimiter)] == '\n')
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

	tmp_fd = open(HEREDOC_TMP, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (tmp_fd < 0)
		error_exit_message(NULL, NULL,
				"Failed to create temporary heredoc file");
	write_heredoc(tmp_fd, delimiter);
	close(tmp_fd);
	cmd->input_fd = open(HEREDOC_TMP, O_RDONLY);
	if (cmd->input_fd < 0)
		error_exit_message(NULL, NULL, "Failed to open temporary heredoc file");
}
