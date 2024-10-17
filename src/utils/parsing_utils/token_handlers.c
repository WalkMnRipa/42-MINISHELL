/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:00:39 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/17 19:04:34 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parsing.h"

int	token_handle_single_quotes(char *input, int i, t_token **head)
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
		new_token = create_token(value, TOKEN_WORD, QUOTE_SINGLE);
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

int	token_handle_double_quotes(char *input, int i, t_token **head)
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
		new_token = create_token(value, TOKEN_WORD, QUOTE_DOUBLE);
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

int	token_handle_word(char *input, int i, t_token **head)
{
	int		start;
	char	*value;
	t_token	*new_token;

	start = i;
	while (input[i] && !ft_isspace(input[i]) && !ft_strchr("|><", input[i]))
		i++;
	value = ft_substr(input, start, i - start);
	if (!value)
		return (i);
	new_token = create_token(value, determine_token_type(value), QUOTE_NONE);
	free(value);
	if (!new_token)
		return (-1);
	add_token(head, new_token);
	return (i - 1);
}

int	check_unclosed_quotes(char *input)
{
	int	i;
	int	in_single_quote;
	int	in_double_quote;

	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (input[i] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		i++;
	}
	if (in_single_quote)
		return (ft_putendl_fd(ERR_SINGLE_QUOTE, 2), 1);
	if (in_double_quote)
		return (ft_putendl_fd(ERR_DOUBLE_QUOTE, 2), 1);
	return (0);
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
