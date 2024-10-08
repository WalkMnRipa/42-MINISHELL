/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_functions.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 16:50:42 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/08 17:40:15 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*ft_create_token(char *value, t_token_type type,
		t_quote_type quote_type)
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

t_token_type	get_token_type(char *value)
{
	if (ft_strncmp(value, "|", 2) == 0)
		return (TOKEN_PIPE);
	else if (ft_strncmp(value, ">", 2) == 0)
		return (TOKEN_REDIR_OUTPUT);
	else if (ft_strncmp(value, ">>", 3) == 0)
		return (TOKEN_REDIR_APPEND);
	else if (ft_strncmp(value, "<", 2) == 0)
		return (TOKEN_REDIR_INPUT);
	else if (ft_strncmp(value, "<<", 3) == 0)
		return (TOKEN_HERE_DOC);
	else
		return (TOKEN_WORD);
}

t_token	*tokenize_input(char *input, t_env *env, int last_exit_status)
{
	t_token	*head;
	int		i;
	int		result;

	head = NULL;
	i = 0;
	while (input[i])
	{
		result = process_token(input, i, &head);
		if (result < 0)
		{
			free_tokens(head);
			return (NULL);
		}
		i = result;
	}
	expand_variables_in_tokens(&head, env, last_exit_status);
	return (head);
}
