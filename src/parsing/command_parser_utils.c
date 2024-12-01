/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parser_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 16:54:27 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/01 22:18:11 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_cmd	*create_command(void)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->input_fd = STDIN_FILENO;
	cmd->output_fd = STDOUT_FILENO;
	cmd->append_output = 0;
	cmd->exit_status = 0;
	cmd->heredocs = NULL; // Initialize heredocs list
	cmd->next = NULL;
	return (cmd);
}

int	add_argument(t_cmd *cmd, char *arg)
{
	int		i;
	char	**new_args;

	i = 0;
	if (cmd->args)
		while (cmd->args[i])
			i++;
	new_args = (char **)malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		return (0);
	i = 0;
	if (cmd->args)
	{
		while (cmd->args[i])
		{
			new_args[i] = cmd->args[i];
			i++;
		}
		free(cmd->args);
	}
	new_args[i] = ft_strdup(arg);
	new_args[i + 1] = NULL;
	cmd->args = new_args;
	return (1);
}

int	handle_redirection(t_cmd *cmd, t_token *token, t_token *next, t_env *env)
{
	char		*old_file;
	static int	heredoc_count = 0;
	char		*heredoc_file;
	char		*count_str;
	int			fd;

	if (!next || next->type != TOKEN_WORD)
	{
		print_syntax_error("newline");
		return (0);
	}
	if (token->type == TOKEN_HEREDOC)
	{
		count_str = ft_itoa(heredoc_count++);
		if (!count_str)
			return (0);
		heredoc_file = ft_strjoin(".heredoc_tmp_", count_str);
		free(count_str);
		if (!heredoc_file)
			return (0);
		if (handle_heredoc_with_file(cmd, next->value, env, heredoc_file))
		{
			free(heredoc_file);
			return (0);
		}
		old_file = cmd->input_file;
		cmd->input_file = heredoc_file;
		free(old_file);
		return (1);
	}
	if (token->type == TOKEN_REDIR_OUT || token->type == TOKEN_REDIR_APPEND)
	{
		// Create/truncate the previous output file if it exists
		if (cmd->output_file)
		{
			fd = open(cmd->output_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			if (fd != -1)
				close(fd);
		}
		// Update to new output file
		old_file = cmd->output_file;
		cmd->output_file = ft_strdup(next->value);
		if (!cmd->output_file)
			return (0);
		// Create the new file if it's a regular redirect (not append)
		if (token->type == TOKEN_REDIR_OUT)
		{
			fd = open(next->value, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			if (fd != -1)
				close(fd);
		}
		cmd->append_output = (token->type == TOKEN_REDIR_APPEND);
		free(old_file);
	}
	else if (token->type == TOKEN_REDIR_IN)
	{
		old_file = cmd->input_file;
		cmd->input_file = ft_strdup(next->value);
		if (!cmd->input_file)
			return (0);
		free(old_file);
	}
	return (1);
}

int	handle_heredoc_with_file(t_cmd *cmd, char *delimiter, t_env *env,
		const char *filename)
{
	int		fd;
	char	*clean_delim;
	int		expand_vars;

	g_signal_received = 0;
	expand_vars = (ft_strchr(delimiter, '\'') == NULL);
	clean_delim = ft_strtrim(delimiter, "'\"");
	if (!clean_delim)
		return (1);
	setup_heredoc_signals();
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		free(clean_delim);
		ft_putendl_fd(ERR_HEREDOC_CREATING, STDERR_FILENO);
		return (1);
	}
	if (write_heredoc(fd, clean_delim, env, expand_vars))
	{
		close(fd);
		unlink(filename);
		free(clean_delim);
		g_signal_received = 0;
		setup_signals();
		return (1);
	}
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
