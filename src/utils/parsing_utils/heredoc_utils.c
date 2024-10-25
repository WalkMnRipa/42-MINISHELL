/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 15:49:43 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/25 15:57:44 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parsing.h"

char	*get_heredoc_content(const char *delimiter)
{
	char	*line;
	char	*content;
	char	*temp;

	content = ft_strdup("");
	if (!content)
		return (NULL);
	while (1)
	{
		ft_putstr_fd(HEREDOC_PROMPT, STDOUT_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
			&& line[ft_strlen(delimiter)] == '\n')
		{
			free(line);
			break ;
		}
		temp = ft_strjoin(content, line);
		free(content);
		free(line);
		if (!temp)
			return (NULL);
		content = temp;
	}
	return (content);
}

int	write_heredoc_to_file(char *content)
{
	int	fd;

	fd = open(HEREDOC_TMP, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_putendl_fd(ERR_CREATING_HERE_DOC, STDERR_FILENO);
		return (-1);
	}
	ft_putstr_fd(content, fd);
	close(fd);
	fd = open(HEREDOC_TMP, O_RDONLY);
	if (fd == -1)
	{
		ft_putendl_fd(ERR_CREATING_HERE_DOC, STDERR_FILENO);
		return (-1);
	}
	return (fd);
}

void	handle_heredoc_parsing(t_cmd *cmd, char *delimiter)
{
	char	*content;
	int		fd;

	content = get_heredoc_content(delimiter);
	if (!content)
		error_exit_message(NULL, NULL, ERR_MALLOC_FAILED);
	fd = write_heredoc_to_file(content);
	free(content);
	if (fd == -1)
		error_exit_message(NULL, NULL, ERR_CREATING_HERE_DOC);
	if (cmd->input_fd != STDIN_FILENO)
		close(cmd->input_fd);
	cmd->input_fd = fd;
}
