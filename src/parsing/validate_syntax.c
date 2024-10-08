/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 16:26:32 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/08 16:54:24 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

static int	process_token(char *input, int i, t_token **head)
{
	int	result;

	if (input[i] == '\'')
		result = handle_single_quotes(input, i, head);
	else if (input[i] == '"')
		result = handle_double_quotes(input, i, head);
	else if (ft_isspace(input[i]))
		result = handle_space(input, i);
	else
		result = handle_word(input, i, head);
	if (result < 0)
		return (ft_handle_parse_error(-result));
	return (result);
}

static t_parse_error	validate_command(t_command *cmd)
{
	t_redirect	*redir;

	if (!cmd->args[0])
		return (ft_handle_parse_error(PARSE_ERROR_NO_COMMAND));
	redir = cmd->redirects;
	while (redir)
	{
		if (!redir->file)
			return (ft_handle_parse_error(PARSE_ERROR_REDIR_NO_FILE));
		redir = redir->next;
	}
	return (PARSE_ERROR_NONE);
}

static void	finalize_parsing(t_command **head, t_command *current)
{
	if (!*head)
		*head = current;
}

t_command	*parse_commands(t_token *tokens)
{
	t_command		*head;
	t_command		*current_command;
	t_parse_error	error;

	head = NULL;
	current_command = NULL;
	while (tokens)
	{
		error = process_token(tokens, &current_command, &head);
		if (error != PARSE_ERROR_NONE)
			return (free_commands(head));
		if (!tokens->next || tokens->next->type == TOKEN_PIPE)
		{
			error = validate_command(current_command);
			if (error != PARSE_ERROR_NONE)
				return (free_commands(head));
		}
		if (tokens->type == TOKEN_REDIR_INPUT
			|| tokens->type == TOKEN_REDIR_OUTPUT
			|| tokens->type == TOKEN_REDIR_APPEND
			|| tokens->type == TOKEN_HERE_DOC)
			tokens = tokens->next;
		tokens = tokens->next;
	}
	return (finalize_parsing(&head, current_command), head);
}
