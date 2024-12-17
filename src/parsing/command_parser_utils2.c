/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parser_utils2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 23:47:10 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/17 20:35:14 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_input_redirect(t_cmd *cmd, t_token *next)
{
	char	*new_file;

	new_file = ft_strdup(next->value);
	if (!new_file)
		return (0);
	cleanup_ptr(cmd->input_file);
	cmd->input_file = new_file;
	return (1);
}

int	handle_output_redirect(t_cmd *cmd, t_token *token, t_token *next)
{
	int		fd;
	char	*new_file;

	if (cmd->output_file)
	{
		fd = open(cmd->output_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd != -1)
			close(fd);
	}
	new_file = ft_strdup(next->value);
	if (!new_file)
		return (0);
	cleanup_ptr(cmd->output_file);
	cmd->output_file = new_file;
	if (token->type == TOKEN_REDIR_OUT)
	{
		fd = open(next->value, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd != -1)
			close(fd);
	}
	cmd->append_output = (token->type == TOKEN_REDIR_APPEND);
	return (1);
}

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
	cmd->heredocs = NULL;
	cmd->next = NULL;
	return (cmd);
}
