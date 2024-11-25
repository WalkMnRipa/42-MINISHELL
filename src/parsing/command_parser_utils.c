/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parser_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 16:54:27 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/25 18:11:29 by ggaribot         ###   ########.fr       */
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

int	handle_redirection(t_cmd *cmd, t_token *token, t_token *next)
{
	char	*old_file;

	if (!next || next->type != TOKEN_WORD)
	{
		print_syntax_error("newline");
		return (0);
	}
	if (token->type == TOKEN_REDIR_OUT || token->type == TOKEN_REDIR_APPEND)
	{
		old_file = cmd->output_file;
		cmd->output_file = ft_strdup(next->value);
		cmd->append_output = (token->type == TOKEN_REDIR_APPEND);
		free(old_file);
	}
	else if (token->type == TOKEN_REDIR_IN)
	{
		old_file = cmd->input_file;
		cmd->input_file = ft_strdup(next->value);
		free(old_file);
	}
	return (1);
}
