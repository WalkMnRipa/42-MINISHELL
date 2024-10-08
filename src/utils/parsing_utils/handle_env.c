/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 17:24:43 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/08 17:29:10 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parsing.h"

int	handle_env_variable(char *input, int i, t_token **head)
{
	int		start;
	char	*var_name;
	char	*var_value;
	t_token	*new_token;

	start = i++;
	while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
		i++;
	var_name = ft_substr(input, start + 1, i - start - 1);
	if (!var_name)
		return (ft_handle_parse_error(PARSE_ERROR_MALLOC));
	var_value = getenv(var_name);
	if (var_value)
	{
		new_token = ft_create_token(var_value, TOKEN_WORD, QUOTE_NONE);
		if (!new_token)
		{
			free(var_name);
			return (ft_handle_parse_error(PARSE_ERROR_MALLOC));
		}
		add_token(head, new_token);
	}
	free(var_name);
	return (i - 1);
}

int	handle_special_variable(char *input, int i, t_token **head,
		int last_exit_status)
{
	char	*value;
	t_token	*new_token;

	if (input[i + 1] == '?')
	{
		value = ft_itoa(last_exit_status);
		i++;
	}
	else if (input[i + 1] == '$')
	{
		value = ft_itoa(getpid());
		i++;
	}
	else
		return (i);
	if (!value)
		return (ft_handle_parse_error(PARSE_ERROR_MALLOC));
	new_token = ft_create_token(value, TOKEN_WORD, QUOTE_NONE);
	free(value);
	if (!new_token)
		return (ft_handle_parse_error(PARSE_ERROR_MALLOC));
	add_token(head, new_token);
	return (i);
}

int	handle_backslash(char *input, int i, t_token **head)
{
	char	escaped_char[2];
	t_token	*new_token;

	if (!input[i + 1])
		return (ft_handle_parse_error(PARSE_ERROR_SYNTAX));
	escaped_char[0] = input[i + 1];
	escaped_char[1] = '\0';
	new_token = ft_create_token(escaped_char, TOKEN_WORD, QUOTE_NONE);
	if (!new_token)
		return (ft_handle_parse_error(PARSE_ERROR_MALLOC));
	add_token(head, new_token);
	return (i + 1);
}
