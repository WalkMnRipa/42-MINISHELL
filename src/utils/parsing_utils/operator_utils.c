/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 16:27:47 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/10 18:19:54 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parsing.h"

int	handle_operators(t_token **tokens)
{
	t_token	*current;
	int		result;

	if (check_syntax_errors(*tokens))
		return (1);
	current = *tokens;
	while (current)
	{
		result = 0;
		if (current->type == TOKEN_PIPE)
			result = handle_pipe(current);
		else if (current->type == TOKEN_REDIR_INPUT)
			result = handle_redir_input(current);
		else if (current->type == TOKEN_REDIR_OUTPUT)
			result = handle_redir_output(current);
		else if (current->type == TOKEN_REDIR_APPEND)
			result = handle_redir_append(current);
		else if (current->type == TOKEN_HERE_DOC)
			result = handle_here_doc(current);
		if (result)
			return (result);
		current = current->next;
	}
	return (0);
}

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
				return (ft_putstr_fd("Syntax error: unexpected token\n", 2), 1);
			expect_word = 0;
		}
		else
		{
			if (current->type != TOKEN_WORD)
			{
				if (!current->next)
					return (ft_putstr_fd("Syntax error: unexpected token\n", 2),
						1);
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
		ft_putstr_fd(ERR_UNEXPECTED_NEWLINE, 2);
		return (1);
	}
	return (modify_token_type(current, TOKEN_HERE_DOC));
}
