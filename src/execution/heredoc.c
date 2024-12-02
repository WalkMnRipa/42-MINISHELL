/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:38:41 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/02 11:24:24 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_heredoc_line(char *line, int fd, t_env *env, int expand_vars)
{
	char	*expanded_line;

	if (expand_vars)
	{
		expanded_line = expand_variables_in_str(ft_strdup(line), env,
				STATE_NORMAL);
		if (expanded_line)
		{
			ft_putstr_fd(expanded_line, fd);
			free(expanded_line);
		}
	}
	else
		ft_putstr_fd(line, fd);
}

static int	check_delimiter(char *line, char *delimiter)
{
	size_t	delimiter_len;

	if (!line || !delimiter)
		return (0);
	delimiter_len = ft_strlen(delimiter);
	return (!ft_strncmp(line, delimiter, delimiter_len)
		&& line[delimiter_len] == '\n');
}

int	write_heredoc(int fd, char *delimiter, t_env *env, int expand_vars)
{
	char	*line;
	int		stdin_backup;

	stdin_backup = dup(STDIN_FILENO);
	if (stdin_backup == -1)
		return (1);
	while (!g_signal_received)
	{
		ft_putstr_fd(HEREDOC_PROMPT, STDERR_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (!line && !g_signal_received)
		{
			write(STDERR_FILENO, "\n", 1);
			dup2(stdin_backup, STDIN_FILENO);
			close(stdin_backup);
			return (1);
		}
		if (!line || check_delimiter(line, delimiter))
		{
			free(line);
			break ;
		}
		handle_heredoc_line(line, fd, env, expand_vars);
		free(line);
	}
	dup2(stdin_backup, STDIN_FILENO);
	close(stdin_backup);
	return (g_signal_received);
}
