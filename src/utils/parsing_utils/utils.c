/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 18:55:18 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/18 16:36:11 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parsing.h"

int	token_handle_space(char *input, int i)
{
	while (input[i] && ft_isspace(input[i]))
		i++;
	return (i - 1);
}

static int	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

int	token_handle_variable(char *input, int i, t_token **head)
{
	int		start;
	char	*value;
	t_token	*new_token;

	start = i++;
	if (input[i] == '?' || input[i] == '$')
		i++;
	else
	{
		while (input[i] && is_valid_var_char(input[i]))
			i++;
	}
	value = ft_substr(input, start, i - start);
	if (!value)
		return (i);
	new_token = create_token(value, TOKEN_VARIABLE, QUOTE_NONE);
	if (!new_token)
		return (free(value), -1);
	add_token(head, new_token);
	free(value);
	return (i - 1);
}
