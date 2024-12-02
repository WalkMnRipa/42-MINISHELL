/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parser_utils2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 23:47:10 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/02 10:55:21 by ggaribot         ###   ########.fr       */
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
