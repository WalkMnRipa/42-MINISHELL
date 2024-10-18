/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 18:47:50 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/17 19:06:45 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

static int	is_redirection(t_token_type type)
{
	return (type == TOKEN_REDIR_INPUT || type == TOKEN_REDIR_OUTPUT
		|| type == TOKEN_REDIR_APPEND || type == TOKEN_HERE_DOC);
}

static int	check_consecutive_operators(t_token *prev, t_token *current)
{
	if (prev && (is_redirection(prev->type) || prev->type == TOKEN_PIPE)
		&& (is_redirection(current->type) || current->type == TOKEN_PIPE))
		return (ft_putendl_fd(ERR_UNEXPECTED_TOKEN, 2), 1);
	return (0);
}

static int	check_unexpected_pipe(t_token *current)
{
	if (!current->next && current->type == TOKEN_PIPE)
		return (ft_putendl_fd(ERR_UNEXPECTED_TOKEN, 2), 1);
	return (0);
}

static int	check_token_syntax(t_token **current, t_token **prev,
		int *expect_word)
{
	if (*expect_word && (*current)->type != TOKEN_WORD)
		return (ft_putendl_fd(ERR_UNEXPECTED_TOKEN, 2), 1);
	else if (!*expect_word && (*current)->type != TOKEN_WORD)
	{
		if (check_unexpected_pipe(*current))
			return (1);
		*expect_word = 1;
	}
	else
		*expect_word = 0;
	*prev = *current;
	*current = (*current)->next;
	return (0);
}

int	check_syntax_errors(t_token *tokens)
{
	t_token	*current;
	t_token	*prev;
	int		expect_word;

	current = tokens;
	prev = NULL;
	expect_word = 1;
	if (current && current->type == TOKEN_PIPE)
		return (ft_putendl_fd(ERR_UNEXPECTED_TOKEN, 2), 1);
	while (current)
	{
		if (check_consecutive_operators(prev, current))
			return (1);
		if (check_token_syntax(&current, &prev, &expect_word))
			return (1);
	}
	if (prev && is_redirection(prev->type))
		return (ft_putendl_fd(ERR_UNEXPECTED_NEWLINE, 2), 1);
	return (0);
}