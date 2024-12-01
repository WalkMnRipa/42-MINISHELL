/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parser_utils2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 23:47:10 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/01 23:47:51 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_input_redirect(t_cmd *cmd, t_token *next)
{
	char	*old_file;

	old_file = cmd->input_file;
	cmd->input_file = ft_strdup(next->value);
	if (!cmd->input_file)
		return (0);
	free(old_file);
	return (1);
}

int	handle_output_redirect(t_cmd *cmd, t_token *token, t_token *next)
{
	int		fd;
	char	*old_file;

	if (cmd->output_file)
	{
		fd = open(cmd->output_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd != -1)
			close(fd);
	}
	old_file = cmd->output_file;
	cmd->output_file = ft_strdup(next->value);
	if (!cmd->output_file)
		return (0);
	if (token->type == TOKEN_REDIR_OUT)
	{
		fd = open(next->value, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd != -1)
			close(fd);
	}
	cmd->append_output = (token->type == TOKEN_REDIR_APPEND);
	free(old_file);
	return (1);
}

static int	setup_heredoc_file(int *fd, char *clean_delim, const char *filename)
{
	*fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (*fd == -1)
	{
		free(clean_delim);
		ft_putendl_fd(ERR_HEREDOC_CREATING, STDERR_FILENO);
		return (1);
	}
	return (0);
}

static int	handle_heredoc_write(int fd, char *clean_delim,
		const char *filename, t_env *env)
{
	int	expand_vars;

	expand_vars = (ft_strchr(clean_delim, '\'') == NULL);
	if (write_heredoc(fd, clean_delim, env, expand_vars))
	{
		close(fd);
		unlink(filename);
		free(clean_delim);
		g_signal_received = 0;
		setup_signals();
		return (1);
	}
	return (0);
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
	if (setup_heredoc_file(&fd, clean_delim, filename))
		return (1);
	if (handle_heredoc_write(fd, clean_delim, filename, env))
		return (1);
	close(fd);
	free(clean_delim);
	cmd->input_fd = open(filename, O_RDONLY);
	if (cmd->input_fd == -1)
	{
		ft_putendl_fd(ERR_HEREDOC_OPEN, STDERR_FILENO);
		unlink(filename);
		return (1);
	}
	return (0);
}
