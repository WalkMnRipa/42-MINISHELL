/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 19:00:03 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/25 16:59:11 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

static int	handle_pipe(t_cmd **current, t_token *token)
{
	if (!token->next)
	{
		print_syntax_error("newline");
		return (0);
	}
	(*current)->next = create_command();
	*current = (*current)->next;
	if (!(*current))
		return (0);
	return (1);
}

static int	process_token(t_cmd **current, t_token **token)
{
	if ((*token)->type == TOKEN_WORD)
	{
		if (!add_argument(*current, (*token)->value))
			return (0);
	}
	else if ((*token)->type == TOKEN_PIPE)
	{
		if (!handle_pipe(current, *token))
			return (0);
	}
	else if ((*token)->type >= TOKEN_REDIR_IN
		&& (*token)->type <= TOKEN_HEREDOC)
	{
		if (!handle_redirection(*current, *token, (*token)->next))
			return (0);
		*token = (*token)->next;
	}
	return (1);
}

t_cmd	*group_tokens_into_commands(t_token *tokens, t_env *env)
{
	t_cmd	*head;
	t_cmd	*current;
	t_token	*token;

	(void)env;
	head = create_command();
	if (!head)
		return (NULL);
	current = head;
	token = tokens;
	while (token)
	{
		if (!process_token(&current, &token))
		{
			free_cmd_list(head);
			return (NULL);
		}
		token = token->next;
	}
	return (head);
}
