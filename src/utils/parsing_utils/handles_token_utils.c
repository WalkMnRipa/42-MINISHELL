/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handles_token_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:00:39 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/03 19:58:10 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parsing.h"

int	handle_single_quotes(char *input, int i, t_token **head)
{
	int		start;
	char	*value;
	t_token	*new_token;

	if (!input || !head)
		return (-1);
	start = i++;
	while (input[i] && input[i] != '\'')
		i++;
	if (input[i] == '\'')
	{
		value = ft_substr(input, start + 1, i - start - 1);
		if (!value)
			return (-1);
		new_token = ft_create_token(value, TOKEN_WORD, QUOTE_SINGLE);
		if (!new_token)
		{
			free(value);
			return (-1);
		}
		add_token(head, new_token);
		free(value);
		return (i);
	}
	return (start);
}

int	handle_double_quotes(char *input, int i, t_token **head)
{
	int		start;
	char	*value;
	t_token	*new_token;

	if (!input || !head)
		return (-1);
	start = i++;
	while (input[i] && input[i] != '"')
		i++;
	if (input[i] == '"')
	{
		value = ft_substr(input, start + 1, i - start - 1);
		if (!value)
			return (-1);
		new_token = ft_create_token(value, TOKEN_WORD, QUOTE_DOUBLE);
		if (!new_token)
		{
			free(value);
			return (-1);
		}
		add_token(head, new_token);
		free(value);
		return (i);
	}
	return (start);
}

int	handle_word(char *input, int i, t_token **head)
{
	int		start;
	char	*value;
	t_token	*new_token;

	start = i;
	while (input[i] && !ft_isspace(input[i]) && input[i] != '\''
		&& input[i] != '"' && input[i] != '|' && input[i] != '<'
		&& input[i] != '>')
		i++;
	value = ft_substr(input, start, i - start);
	if (!value)
		return (i);
	new_token = ft_create_token(value, get_token_type(value), QUOTE_NONE);
	free(value);
	if (new_token)
		add_token(head, new_token);
	return (i - 1);
}

int	handle_space(char *input, int i)
{
	while (input[i] && ft_isspace(input[i]))
		i++;
	return (i - 1);
}
