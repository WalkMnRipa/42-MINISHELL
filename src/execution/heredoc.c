/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:38:41 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/16 18:31:16 by ggaribot         ###   ########.fr       */
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
			free(expanded_line);
		}
	}
	else
		write(fd, line, ft_strlen(line));
}

static int	check_delimiter(const char *line, const char *delimiter)
{
	char	*trimmed_line;
	int		result;
	size_t	len;

	if (!line || !delimiter)
		return (0);
	trimmed_line = ft_strdup(line);
	if (!trimmed_line)
		return (0);
	len = ft_strlen(trimmed_line);
	if (len > 0 && trimmed_line[len - 1] == '\n')
		trimmed_line[len - 1] = '\0';
	result = (ft_strcmp(trimmed_line, delimiter) == 0);
	free(trimmed_line);
	return (result);
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

static int	process_heredoc_line(char *line, int fd, t_env *env,
		t_heredoc_data *data)
{
	int	ret;

	if (!line)
	{
		ret = handle_heredoc_cleanup(data->stdin_backup, 1);
		close(fd);
		return (ret);
	}
	if (check_delimiter(line, data->delimiter))
	{
		free(line);
		ret = handle_heredoc_cleanup(data->stdin_backup, 0);
		close(fd);
		return (ret);
	}
	handle_heredoc_line(line, fd, env, data->expand_vars);
	free(line);
	return (-1);
}

int	write_heredoc(int fd, char *delimiter, t_env *env, int expand_vars)
{
	char			*line;
	int				ret;
	t_heredoc_data	data;

	data.stdin_backup = dup(STDIN_FILENO);
	if (data.stdin_backup == -1)
		return (1);
	data.delimiter = delimiter;
	data.expand_vars = expand_vars;
	g_signal_received = 0;
	setup_heredoc_signals();
	while (!g_signal_received)
	{
		ft_putstr_fd(HEREDOC_PROMPT, STDERR_FILENO);
		line = get_next_line(STDIN_FILENO);
		ret = process_heredoc_line(line, fd, env, &data);
		if (ret >= 0)
			return (ret);
	}
	ret = handle_heredoc_cleanup(data.stdin_backup, 0);
	close(fd);
	setup_signals();
	return (ret);
}
