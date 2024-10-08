/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handles_token_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:00:39 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/08 17:25:00 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parsing.h"

int	handle_single_quotes(char *input, int i, t_token **head)
{
	int		start;
	char	*value;
	t_token	*new_token;

	start = i++;
	while (input[i] && input[i] != '\'')
		i++;
	if (!input[i])
		return (ft_handle_parse_error(PARSE_ERROR_UNCLOSED_QUOTE));
	value = ft_substr(input, start + 1, i - start - 1);
	if (!value)
		return (ft_handle_parse_error(PARSE_ERROR_MALLOC));
	new_token = ft_create_token(value, TOKEN_WORD, QUOTE_SINGLE);
	free(value);
	if (!new_token)
		return (ft_handle_parse_error(PARSE_ERROR_MALLOC));
	add_token(head, new_token);
	return (i);
}

int	handle_double_quotes(char *input, int i, t_token **head)
{
	int		start;
	char	*value;
	t_token	*new_token;

	start = i++;
	while (input[i] && input[i] != '"')
		i++;
	if (!input[i])
		return (ft_handle_parse_error(PARSE_ERROR_UNCLOSED_QUOTE));
	value = ft_substr(input, start + 1, i - start - 1);
	if (!value)
		return (ft_handle_parse_error(PARSE_ERROR_MALLOC));
	new_token = ft_create_token(value, TOKEN_WORD, QUOTE_DOUBLE);
	free(value);
	if (!new_token)
		return (ft_handle_parse_error(PARSE_ERROR_MALLOC));
	add_token(head, new_token);
	return (i);
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
		return (-ft_handle_parse_error(PARSE_ERROR_MALLOC));
	new_token = ft_create_token(value, get_token_type(value), QUOTE_NONE);
	if (!new_token)
	{
		free(value);
		return (-ft_handle_parse_error(PARSE_ERROR_MALLOC));
	}
	add_token(head, new_token);
	free(value);
	return (i - 1);
}

int	handle_space(char *input, int i)
{
	while (input[i] && ft_isspace(input[i]))
		i++;
	return (i - 1);
}

t_parse_error	check_token_syntax(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == TOKEN_PIPE)
		{
			if (!tokens->next || tokens->next->type == TOKEN_PIPE)
				return (ft_handle_parse_error(PARSE_ERROR_SYNTAX));
		}
		else if (tokens->type == TOKEN_REDIR_INPUT
			|| tokens->type == TOKEN_REDIR_OUTPUT
			|| tokens->type == TOKEN_REDIR_APPEND
			|| tokens->type == TOKEN_HERE_DOC)
		{
			if (!tokens->next || tokens->next->type != TOKEN_WORD)
				return (ft_handle_parse_error(PARSE_ERROR_SYNTAX));
		}
		tokens = tokens->next;
	}
	return (PARSE_ERROR_NONE);
}
