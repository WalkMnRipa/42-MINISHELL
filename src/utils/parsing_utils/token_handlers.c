/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:00:39 by jcohen            #+#    #+#             */
/*   Updated: 2024/11/02 22:28:17 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parsing.h"

int	token_handle_single_quotes(char *input, int i, t_token **head)
{
	int		start;
	int		end;
	char	*value;
	t_token	*new_token;

	if (!input[i + 1])
		return (-1);
	start = i + 1;
	end = start;
	while (input[end] && input[end] != '\'')
		end++;
	if (!input[end])
		return (-1);
	value = ft_substr(input, start, end - start);
	if (!value)
		return (-1);
	new_token = create_token(value, TOKEN_WORD, QUOTE_SINGLE);
	free(value);
	if (!new_token)
		return (-1);
	add_token(head, new_token);
	return (end);
}

int	token_handle_double_quotes(char *input, int i, t_token **head, t_env *env)
{
	char	*content;
	char	*expanded;
	t_token	*new_token;

	if (!input[i + 1])
		return (-1);
	content = get_quoted_content(input, i + 1, '"');
	if (!content)
		return (-1);
	expanded = expand_variables_in_str(content, env, QUOTE_DOUBLE);
	free(content);
	if (!expanded)
		return (-1);
	new_token = create_token(expanded, TOKEN_WORD, QUOTE_DOUBLE);
	free(expanded);
	if (!new_token)
		return (-1);
	add_token(head, new_token);
	return (get_quote_end(input, i + 1, '"'));
}

static int	get_word_end(char *input, int i, t_quote_type *quote_type)
{
	int	current;

	current = i;
	while (input[current])
	{
		if (input[current] == '\'' && *quote_type == QUOTE_NONE)
			*quote_type = QUOTE_SINGLE;
		else if (input[current] == '"' && *quote_type == QUOTE_NONE)
			*quote_type = QUOTE_DOUBLE;
		else if ((input[current] == '\'' && *quote_type == QUOTE_SINGLE)
				|| (input[current] == '"' && *quote_type == QUOTE_DOUBLE))
			*quote_type = QUOTE_NONE;
		else if (ft_isspace(input[current]) && *quote_type == QUOTE_NONE)
			break ;
		current++;
	}
	return (current);
}

int	token_handle_word(char *input, int i, t_token **head, t_env *env)
{
	int				end;
	char			*value;
	char			*expanded;
	t_token			*new_token;
	t_quote_type	quote_type;

	quote_type = QUOTE_NONE;
	end = get_word_end(input, i, &quote_type);
	if (quote_type != QUOTE_NONE)
		return (ft_strlen(input) - 1);
	value = ft_substr(input, i, end - i);
	if (!value)
		return (-1);
	expanded = expand_quoted_word(value, env);
	free(value);
	if (!expanded)
		return (-1);
	if (ft_strchr(value, ';'))
		return (free(value), ft_putendl_fd(ERR_UNEXPECTED_SEMICOL, 2), -1);
	new_token = create_token(expanded, TOKEN_WORD, QUOTE_NONE);
	free(expanded);
	if (!new_token)
		return (-1);
	add_token(head, new_token);
	return (end - 1);
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
