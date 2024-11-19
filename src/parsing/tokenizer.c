/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 00:43:06 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/19 00:43:08 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

void	init_process_state(t_process_state *ps, char *input, t_token **head,
		t_env *env)
{
	ps->input = input;
	ps->head = head;
	ps->env = env;
	ps->processed_input = NULL;
	ps->in_quotes = 0;
	ps->quote_char = 0;
}

t_token	*create_token(char *value, t_token_type type, t_quote_type quote_type)
{
	t_token	*token;

	if (!value)
		return (NULL);
	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->type = type;
	token->quote_type = quote_type;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **head, t_token *new_token)
{
	t_token	*current;

	if (!head || !new_token)
		return ;
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

t_token	*tokenizer(char *input, t_env *env)
{
	t_token *head;
	t_process_state ps;
	char *temp;

	if (!input)
		return (NULL);
	head = NULL;
	init_process_state(&ps, input, &head, env);

	// Step 1: Process quotes
	ps.processed_input = process_quotes(input);
	if (!ps.processed_input)
		return (NULL);

	// Step 2: Process variables
	temp = process_variables(ps.processed_input, env, 0);
	free(ps.processed_input);
	ps.processed_input = temp;
	if (!ps.processed_input)
		return (NULL);

	// Step 3: Process operators
	if (process_operators(&ps) < 0)
	{
		free(ps.processed_input);
		return (free_tokens(head), NULL);
	}

	// Step 4: Process words
	if (process_words(&ps) < 0)
	{
		free(ps.processed_input);
		return (free_tokens(head), NULL);
	}

	free(ps.processed_input);
	return (head);
}