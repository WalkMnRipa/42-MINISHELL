/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handle_variable.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:24:52 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/23 15:59:38 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parsing.h"

int	token_handle_variable(char *input, int i, t_token **head, t_env *env)
{
	int		start;
	char	*value;
	t_token	*new_token;
	char	*env_value;

	if (!input || !head || input[i] != '$')
		return (-1);
	start = i + 1;
	if (!input[start] || ft_isspace(input[start]))
	{
		new_token = create_token("$", TOKEN_WORD, QUOTE_NONE);
		if (!new_token)
			return (-1);
		add_token(head, new_token);
		return (i);
	}
	if (input[start] == '?')
	{
		value = ft_itoa(env->last_exit_status);
		if (!value)
			return (-1);
		new_token = create_token(value, TOKEN_WORD, QUOTE_NONE);
		free(value);
		if (!new_token)
			return (-1);
		add_token(head, new_token);
		return (start);
	}
	if (input[start] == '$')
	{
		value = ft_itoa(getpid());
		if (!value)
			return (-1);
		new_token = create_token(value, TOKEN_WORD, QUOTE_NONE);
		free(value);
		if (!new_token)
			return (-1);
		add_token(head, new_token);
		return (start);
	}
	start = i + 1;
	i = start;
	while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
		i++;
	if (i == start) // $ suivi d'un caractère invalide
	{
		new_token = create_token("$", TOKEN_WORD, QUOTE_NONE);
		if (!new_token)
			return (-1);
		add_token(head, new_token);
		return (start - 1);
	}
	value = ft_substr(input, start, i - start);
	if (!value)
		return (-1);
	env_value = get_env_value(env, value);
	free(value);
	new_token = create_token(env_value ? env_value : "", TOKEN_WORD,
			QUOTE_NONE);
	if (!new_token)
		return (-1);
	add_token(head, new_token);
	return (i - 1);
}
