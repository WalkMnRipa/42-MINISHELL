/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handle_variable.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:24:52 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/22 18:10:35 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parsing.h"

int	token_handle_variable(char *input, int i, t_token **head, t_env *env)
{
	int		start;
	char	*value;
	char	*var_name;
	t_token	*new_token;

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
		return (start + 1);
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
		return (start + 1);
	}
	// Variables normales
	while (input[start] && (ft_isalnum(input[start]) || input[start] == '_'))
		start++;
	var_name = ft_substr(input, i + 1, start - (i + 1));
	if (!var_name)
		return (-1);
	value = get_env_value(env, var_name);
	new_token = create_token(value ? value : "", TOKEN_WORD, QUOTE_NONE);
	free(var_name);
	if (!new_token)
		return (-1);
	add_token(head, new_token);
	return (start - 1);
}
