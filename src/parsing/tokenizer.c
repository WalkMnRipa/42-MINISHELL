/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 18:30:32 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/22 18:09:51 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

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

t_token_type	determine_token_type(char *value)
{
	if (ft_strcmp(value, "|") == 0)
		return (TOKEN_PIPE);
	else if (ft_strcmp(value, "<") == 0)
		return (TOKEN_REDIR_INPUT);
	else if (ft_strcmp(value, ">") == 0)
		return (TOKEN_REDIR_OUTPUT);
	else if (ft_strcmp(value, ">>") == 0)
		return (TOKEN_REDIR_APPEND);
	else if (ft_strcmp(value, "<<") == 0)
		return (TOKEN_HERE_DOC);
	else
		return (TOKEN_WORD);
}

int	handle_token(char *input, int i, t_token **head, t_env *env)
{
	if (input[i] == '\'')
		return (token_handle_single_quotes(input, i, head));
	else if (input[i] == '"')
		return (token_handle_double_quotes(input, i, head, env));
	else if (ft_isspace(input[i]))
		return (token_handle_space(input, i));
	else if (ft_strchr("|><", input[i]))
		return (token_handle_redirection(input, i, head));
	else if (input[i] == '$')
		return (token_handle_variable(input, i, head, env));
	else
		return (token_handle_word(input, i, head, env));
}

t_token	*tokenizer(char *input, t_env *env)
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
		new_i = handle_token(input, i, &head, env);
		if (new_i < 0)
		{
			free_tokens(head);
			return (NULL);
		}
		i = new_i + 1;
	}
	if (check_syntax_errors(head))
	{
		free_tokens(head);
		return (NULL);
	}
	return (head);
}
