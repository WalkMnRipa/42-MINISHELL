/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parser_utils2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 23:47:10 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/17 16:33:16 by ggaribot         ###   ########.fr       */
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
