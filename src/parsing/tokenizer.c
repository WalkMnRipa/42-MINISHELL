/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 18:30:32 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/07 12:59:34 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

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

t_token	*ft_tokenizer(char *input)
{
	t_token	*head;
	int		i;
	int		new_i;

	if (!input)
		return (NULL);
	head = NULL;
	i = 0;
	while (input[i])
	{
		if (input[i] == '\'')
			new_i = handle_single_quotes(input, i, &head);
		else if (input[i] == '"')
			new_i = handle_double_quotes(input, i, &head);
		else if (ft_isspace(input[i]))
			new_i = handle_space(input, i);
		else
			new_i = handle_word(input, i, &head);
		if (new_i < 0)
			return (free_tokens(head), NULL);
		i = new_i + 1;
	}
	return (head);
}
