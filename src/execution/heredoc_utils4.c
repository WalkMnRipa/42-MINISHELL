/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 10:54:39 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/02 15:19:42 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	setup_input_fd(t_cmd *cmd, const char *filename)
{
	cmd->input_fd = open(filename, O_RDONLY);
	if (cmd->input_fd == -1)
	{
		ft_putendl_fd(ERR_HEREDOC_OPEN, STDERR_FILENO);
		unlink(filename);
		return (1);
	}
	return (0);
}

static int	write_to_heredoc(int fd, char *clean_delim, t_env *env,
		const char *filename)
{
	int	result;

	result = write_heredoc(fd, clean_delim, env, ft_strchr(clean_delim,
				'\'') == NULL);
	if (result)
	{
		close(fd);
		free(clean_delim);
		unlink(filename);
		g_signal_received = 0;
		setup_signals();
		return (1);
	}
	return (0);
}

static int	open_heredoc_file(const char *filename)
{
	int	fd;

	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_putendl_fd(ERR_HEREDOC_CREATING, STDERR_FILENO);
		return (-1);
	}
	return (fd);
}

int	handle_heredoc_with_file(t_cmd *cmd, char *delimiter, t_env *env,
		const char *filename)
{
	int		fd;
	char	*clean_delim;

	g_signal_received = 0;
	clean_delim = ft_strtrim(delimiter, "'\"");
	if (!clean_delim)
		return (1);
	setup_heredoc_signals();
	fd = open_heredoc_file(filename);
	if (fd == -1)
	{
		free(clean_delim);
		return (1);
	}
	if (write_to_heredoc(fd, clean_delim, env, filename))
		return (1);
	close(fd);
	free(clean_delim);
	return (setup_input_fd(cmd, filename));
}
