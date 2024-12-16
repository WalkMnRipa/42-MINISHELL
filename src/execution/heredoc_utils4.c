/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 10:54:39 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/16 18:29:21 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <sys/resource.h>

static int	count_total_heredocs(t_cmd *cmd)
{
	t_cmd		*current;
	t_heredoc	*heredoc;
	int			count;

	count = 0;
	current = cmd;
	while (current)
	{
		heredoc = current->heredocs;
		while (heredoc)
		{
			count++;
			heredoc = heredoc->next;
		}
		current = current->next;
	}
	return (count);
}

static int	check_fd_limit(t_cmd *cmd)
{
	struct rlimit	rlim;
	int				total_heredocs;
	int				needed_fds;

	if (getrlimit(RLIMIT_NOFILE, &rlim) != 0)
		return (0);
	total_heredocs = count_total_heredocs(cmd);
	needed_fds = total_heredocs * 3;
	if (needed_fds > (int)(rlim.rlim_cur - 20))
	{
		ft_putendl_fd("minishell: Too many heredocs", 2);
		return (0);
	}
	return (1);
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

static void	cleanup_heredoc(int fd, char *clean_delim, const char *filename)
{
	if (fd != -1)
		close(fd);
	if (clean_delim)
		free(clean_delim);
	if (filename)
		unlink(filename);
	g_signal_received = 0;
	setup_signals();
}

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

static int	write_to_file(int fd, char *clean_delim, t_env *env,
		const char *filename)
{
	int	result;
	int	expand_vars;

	expand_vars = (ft_strchr(clean_delim, '\'') == NULL);
	result = write_heredoc(fd, clean_delim, env, expand_vars);
	if (result)
	{
		cleanup_heredoc(fd, clean_delim, filename);
		return (1);
	}
	return (0);
}

int	handle_heredoc_with_file(t_cmd *cmd, char *delimiter, t_env *env,
		const char *filename)
{
	int		fd;
	char	*clean_delim;

	if (!check_fd_limit(cmd))
		return (1);
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
	if (write_to_file(fd, clean_delim, env, filename))
		return (1);
	close(fd);
	free(clean_delim);
	return (setup_input_fd(cmd, filename));
}
