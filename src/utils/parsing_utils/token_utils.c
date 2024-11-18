/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 18:52:59 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/18 15:31:21 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parsing.h"

static void	free_next_token(t_token *current, t_token *next)
{
	current->next = next->next;
	free(next->value);
	free(next);
}

static int	join_tokens(t_token *current, t_token *next)
{
	char	*joined;

	joined = ft_strjoin(current->value, next->value);
	if (!joined)
		return (0);
	free(current->value);
	current->value = joined;
	free_next_token(current, next);
	return (1);
}

void	join_adjacent_word_tokens(t_token **head)
{
	t_token	*current;
	t_token	*next;

	if (!head || !*head)
		return ;
	current = *head;
	while (current && current->next)
	{
		next = current->next;
		if (current->type == TOKEN_WORD && next->type == TOKEN_WORD)
		{
			if (!join_tokens(current, next))
				return ;
		}
		else
			current = current->next;
	}
}

void	join_quoted_word_tokens(t_token **head)
{
	t_token	*current;
	t_token	*next;

	if (!head || !*head)
		return ;
	current = *head;
	if (!current->next)
		return ;
	current = current->next;
	while (current && current->next)
	{
		next = current->next;
		if (current->type == TOKEN_WORD && next->type == TOKEN_WORD
			&& (current->quote_type == QUOTE_DOUBLE
				|| next->quote_type == QUOTE_DOUBLE))
		{
			if (!join_tokens(current, next))
				return ;
		}
		else
			current = current->next;
	}
}
