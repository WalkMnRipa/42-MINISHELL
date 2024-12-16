/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:38:41 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/02 15:09:42 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_heredoc_line(char *line, int fd, t_env *env, int expand_vars)
{
	char	*expanded_line;

	if (!line)
		return ;
	if (expand_vars)
	{
		expanded_line = expand_variables_in_str(ft_strdup(line), env,
				STATE_NORMAL);
		if (expanded_line)
		{
			write(fd, expanded_line, ft_strlen(expanded_line));
			write(fd, "\n", 1);
			free(expanded_line);
		}
	}
	else
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
	}
}

static int	check_delimiter(char *line, char *delimiter)
{
	if (!line || !delimiter)
		return (0);
	if (ft_strlen(line) > 0 && line[ft_strlen(line) - 1] == '\n')
		line[ft_strlen(line) - 1] = '\0';
	return (ft_strcmp(line, delimiter) == 0);
}

static int	handle_heredoc_cleanup(int stdin_backup, int show_eof)
{
	if (show_eof && !g_signal_received)
		ft_putendl_fd("minishell: warning: here-document delimited by EOF",
			STDERR_FILENO);
	dup2(stdin_backup, STDIN_FILENO);
	close(stdin_backup);
	setup_signals();
	return (g_signal_received);
}

int	write_heredoc(int fd, char *delimiter, t_env *env, int expand_vars)
{
	char	*line;
	int		stdin_backup;

	stdin_backup = dup(STDIN_FILENO);
	if (stdin_backup == -1)
		return (1);
	g_signal_received = 0;
	setup_heredoc_signals();
	while (!g_signal_received)
	{
		ft_putstr_fd("> ", STDERR_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			return (handle_heredoc_cleanup(stdin_backup, 1));
		if (check_delimiter(line, delimiter))
		{
			free(line);
			return (handle_heredoc_cleanup(stdin_backup, 0));
		}
		handle_heredoc_line(line, fd, env, expand_vars);
		free(line);
	}
	return (handle_heredoc_cleanup(stdin_backup, 0));
}
