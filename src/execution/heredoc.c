/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:38:41 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/01 15:20:17 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_heredoc_signal(int sig)
{
	if (sig == SIGINT)
	{
		g_signal_received = 1;
		write(STDERR_FILENO, "\n", 1);
		close(0);
	}
}

static void	setup_heredoc_signals(void)
{
	signal(SIGINT, handle_heredoc_signal);
	signal(SIGQUIT, SIG_IGN);
}

static int	write_heredoc(int fd, char *delimiter, t_env *env, int expand_vars)
{
	char	*line;
	int		stdin_backup;

	stdin_backup = dup(STDIN_FILENO);
	if (stdin_backup == -1)
		return (1);
	while (!g_signal_received)
	{
		ft_putstr_fd("> ", 2);
		line = get_next_line(0);
		if (!line && !g_signal_received)
		{
			write(STDERR_FILENO, "\n", 1);
			dup2(stdin_backup, STDIN_FILENO);
			close(stdin_backup);
			return (1);
		}
		if (g_signal_received)
		{
			free(line);
			dup2(stdin_backup, STDIN_FILENO);
			close(stdin_backup);
			return (1);
		}
		if (!ft_strncmp(line, delimiter, ft_strlen(delimiter))
			&& line[ft_strlen(delimiter)] == '\n')
		{
			free(line);
			break ;
		}
		if (expand_vars)
			line = expand_variables_in_str(line, env, STATE_NORMAL);
		ft_putstr_fd(line, fd);
		free(line);
	}
	dup2(stdin_backup, STDIN_FILENO);
	close(stdin_backup);
	return (g_signal_received);
}

int	handle_heredoc(t_cmd *cmd, char *delimiter, t_env *env)
{
	int fd;
	char *clean_delim;
	int expand_vars;
	int status;

	g_signal_received = 0;
	expand_vars = (ft_strchr(delimiter, '\'') == NULL);
	clean_delim = ft_strtrim(delimiter, "'\"");
	if (!clean_delim)
		return (1);
	setup_heredoc_signals();
	fd = open(HEREDOC_TMP, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		free(clean_delim);
		ft_putendl_fd(ERR_HEREDOC_CREATING, STDERR_FILENO);
		return (1);
	}
	status = write_heredoc(fd, clean_delim, env, expand_vars);
	close(fd);
	free(clean_delim);
	if (status)
	{
		unlink(HEREDOC_TMP);
		g_signal_received = 0;
		setup_signals();
		return (1);
	}
	cmd->input_fd = open(HEREDOC_TMP, O_RDONLY);
	if (cmd->input_fd == -1)
	{
		ft_putendl_fd(ERR_HEREDOC_OPEN, STDERR_FILENO);
		unlink(HEREDOC_TMP);
		setup_signals();
		return (1);
	}
	setup_signals();
	return (0);
}