/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handler_variable.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 17:28:23 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/20 01:51:36 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parsing.h"

static int	handle_empty_variable(t_token **head, int start)
{
	t_token	*new_token;

	new_token = create_token("$", TOKEN_WORD, QUOTE_NONE);
	if (!new_token)
		return (-1);
	add_token(head, new_token);
	return (start);
}

static int	get_variable_end(char *input, int i)
{
	if (input[i] == '?' || input[i] == '$')
		return (i + 1);
	while (input[i] && is_valid_var_char(input[i]))
		i++;
	return (i);
}

static int	create_variable_token(char *input, int start, int end,
		t_token **head)
{
	char	*value;
	t_token	*new_token;

	value = ft_substr(input, start, end - start);
	if (!value)
		return (end);
	new_token = create_token(value, TOKEN_VARIABLE, QUOTE_NONE);
	if (!new_token)
	{
		free(value);
		return (-1);
	}
	add_token(head, new_token);
	free(value);
	return (end - 1);
}

int	token_handle_variable(char *input, int i, t_token **head)
{
	int	start;
	int	end;

	start = i++;
	if (!input[i] || ft_isspace(input[i]))
		return (handle_empty_variable(head, start));
	end = get_variable_end(input, i);
	return (create_variable_token(input, start, end, head));
}
