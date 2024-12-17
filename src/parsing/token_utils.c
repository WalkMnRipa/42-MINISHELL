/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:59:19 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/17 16:34:23 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*create_token(t_token_type type, char *value)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
	{
		cleanup_ptr(value);
		return (NULL);
	}
	new_token->type = type;
	new_token->value = value;
	new_token->quote_type = STATE_NORMAL;
	new_token->next = NULL;
	return (new_token);
}

void	add_token(t_token **head, t_token *new_token)
{
	t_token	*current;

	if (!*head)
	{
		*head = new_token;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

void	free_token(t_token *token)
{
	if (!token)
		return ;
	cleanup_ptr(token->value);
	cleanup_ptr(token);
}

void	free_tokens(t_token *head)
{
	t_token	*current;
	t_token	*next;

	current = head;
	while (current)
	{
		next = current->next;
		cleanup_ptr(current->value);
		cleanup_ptr(current);
		current = next;
	}
}
