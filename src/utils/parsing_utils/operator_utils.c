/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 16:27:47 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/15 14:35:37 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parsing.h"

int	check_syntax_errors(t_token *tokens)
{
	t_token	*current;
	int		expect_word;

	current = tokens;
	expect_word = 1;
	while (current)
	{
		if (expect_word)
		{
			if (current->type != TOKEN_WORD)
				return (ft_putendl_fd(ERR_UNEXPECTED_TOKEN, 2), 1);
			expect_word = 0;
		}
		else
		{
			if (current->type != TOKEN_WORD)
			{
				if (!current->next)
					return (ft_putendl_fd(ERR_UNEXPECTED_TOKEN, 2), 1);
				expect_word = 1;
			}
		}
		current = current->next;
	}
	return (0);
}

int	handle_here_doc(t_token *current)
{
	if (!current->next)
	{
		ft_putendl_fd(ERR_UNEXPECTED_NEWLINE, 2);
		return (1);
	}
	return (modify_token_type(current, TOKEN_HERE_DOC));
}

int	handle_token(t_token *token)
{
	if (token->type == TOKEN_PIPE)
		return (handle_pipe(token));
	else if (token->type == TOKEN_REDIR_INPUT)
		return (handle_redir_input(token));
	else if (token->type == TOKEN_REDIR_OUTPUT)
		return (handle_redir_output(token));
	else if (token->type == TOKEN_REDIR_APPEND)
		return (handle_redir_append(token));
	else if (token->type == TOKEN_HERE_DOC)
		return (handle_here_doc(token));
	return (0);
}

int	handle_operators(t_token **tokens)
{
	t_token *current;

	if (check_syntax_errors(*tokens))
		return (1);
	current = *tokens;
	while (current)
	{
		if (handle_token(current))
			return (1);
		current = current->next;
	}
	return (0);
}