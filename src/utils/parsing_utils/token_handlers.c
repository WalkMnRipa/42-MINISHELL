/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:00:39 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/22 18:20:31 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parsing.h"

int	token_handle_single_quotes(char *input, int i, t_token **head)
{
	int		start;
	char	*value;
	t_token	*new_token;
	int		end;

	start = i + 1;
	end = start;
	while (input[end] && input[end] != '\'')
		end++;
	// Si pas de quote fermante, on inclut le reste de la string
	if (!input[end])
		value = ft_substr(input, i, ft_strlen(input) - i);
	else
		value = ft_substr(input, i, end + 1 - i);
	if (!value)
		return (-1);
	new_token = create_token(value, TOKEN_WORD, QUOTE_SINGLE);
	free(value);
	if (!new_token)
		return (-1);
	add_token(head, new_token);
	return (input[end] ? end : ft_strlen(input) - 1);
}

int	token_handle_double_quotes(char *input, int i, t_token **head, t_env *env)
{
	int		start;
	char	*value;
	t_token	*new_token;
	int		end;
	char	*expanded;

	start = i + 1;
	end = start;
	while (input[end] && input[end] != '"')
		end++;
	// Si pas de quote fermante, on inclut le reste de la string
	if (!input[end])
		value = ft_substr(input, i, ft_strlen(input) - i);
	else
		value = ft_substr(input, i, end + 1 - i);
	if (!value)
		return (-1);
	expanded = expand_variables_in_str(value, env, QUOTE_DOUBLE);
	free(value);
	if (!expanded)
		return (-1);
	new_token = create_token(expanded, TOKEN_WORD, QUOTE_DOUBLE);
	free(expanded);
	if (!new_token)
		return (-1);
	add_token(head, new_token);
	return (input[end] ? end : ft_strlen(input) - 1);
}

int	token_handle_word(char *input, int i, t_token **head, t_env *env)
{
	int		start;
	char	*value;
	t_token	*new_token;
	int		current;
	char	quote;
	char	*expanded;

	start = i;
	current = i;
	// Check if the word starts with a quote
	if (input[i] == '\'' || input[i] == '"')
	{
		quote = input[i];
		current++;
		// Search for closing quote
		while (input[current] && input[current] != quote)
			current++;
		// If no closing quote found, treat entire input as literal
		if (!input[current])
		{
			value = ft_strdup(input + i);
			if (!value)
				return (-1);
			new_token = create_token(value, TOKEN_WORD, QUOTE_NONE);
			free(value);
			if (!new_token)
				return (-1);
			add_token(head, new_token);
			return (ft_strlen(input) - 1);
		}
	}
	// Normal word processing for other cases
	while (input[current] && !ft_isspace(input[current]) && !ft_strchr("<>|",
			input[current]))
		current++;
	value = ft_substr(input, start, current - start);
	if (!value)
		return (-1);
	if (input[start] != '\'' && input[start] != '"')
	{
		expanded = expand_variables_in_str(value, env, QUOTE_NONE);
		free(value);
		value = expanded;
	}
	if (!value)
		return (-1);
	new_token = create_token(value, TOKEN_WORD, QUOTE_NONE);
	free(value);
	if (!new_token)
		return (-1);
	add_token(head, new_token);
	return (current - 1);
}

int	token_handle_redirection(char *input, int i, t_token **head)
{
	char	*value;
	t_token	*new_token;
	int		len;

	len = 1;
	if (input[i + 1] && (ft_strncmp(input + i, ">>", 2) == 0 || ft_strncmp(input
				+ i, "<<", 2) == 0))
		len = 2;
	value = ft_substr(input, i, len);
	if (!value)
		return (-1);
	new_token = create_token(value, determine_token_type(value), QUOTE_NONE);
	free(value);
	if (!new_token)
		return (-1);
	add_token(head, new_token);
	return (i + len - 1);
}
