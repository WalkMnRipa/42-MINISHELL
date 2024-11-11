/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 18:52:59 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/11 18:55:09 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parsing.h"

void	join_adjacent_word_tokens(t_token **head)
{
	t_token	*current;
	t_token	*next;
	char	*joined;

	if (!head || !*head)
		return ;
	current = *head;
	while (current && current->next)
	{
		next = current->next;
		if (current->type == TOKEN_WORD && next->type == TOKEN_WORD)
		{
			// Join the tokens without adding extra space
			joined = ft_strjoin(current->value, next->value);
			if (!joined)
				return ;
			// Replace current token's value and remove next token
			free(current->value);
			current->value = joined;
			current->next = next->next;
			free(next->value);
			free(next);
		}
		else
		{
			current = current->next;
		}
	}
}

void	join_quoted_word_tokens(t_token **head)
{
	t_token	*current;
	t_token	*next;
	char	*joined;

	if (!head || !*head)
		return ;
	// Skip the first token (command name)
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
			// Join only if either token is quoted
			joined = ft_strjoin(current->value, next->value);
			if (!joined)
				return ;
			free(current->value);
			current->value = joined;
			current->next = next->next;
			free(next->value);
			free(next);
		}
		else
		{
			current = current->next;
		}
	}
}
