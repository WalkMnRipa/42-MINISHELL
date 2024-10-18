/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handler_variable.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 17:28:23 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/18 17:30:30 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parsing.h"

static int	handle_special_variable(char *input, int i)
{
	if (input[i] == '?' || input[i] == '$')
		return (i + 1);
	return (i);
}

static int	handle_standard_variable(char *input, int i)
{
	while (input[i] && is_valid_var_char(input[i]))
		i++;
	return (i);
}

static int	handle_empty_variable(t_token **head, int start)
{
	char	*value;
	t_token	*new_token;

	value = ft_strdup("$");
	new_token = create_token(value, TOKEN_WORD, QUOTE_NONE);
	if (!new_token)
		return (free(value), -1);
	add_token(head, new_token);
	free(value);
	return (start);
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
		return (free(value), -1);
	add_token(head, new_token);
	free(value);
	return (end - 1);
}

int	token_handle_variable(char *input, int i, t_token **head)
{
	int	start;

	start = i++;
	if (!input[i] || ft_isspace(input[i]))
		return (handle_empty_variable(head, start));
	else if (input[i] == '?' || input[i] == '$')
		i = handle_special_variable(input, i);
	else
		i = handle_standard_variable(input, i);
	return (create_variable_token(input, start, i, head));
}
