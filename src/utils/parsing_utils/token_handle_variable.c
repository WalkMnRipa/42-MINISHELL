/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handle_variable.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:24:52 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/23 19:18:46 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parsing.h"

static int	handle_empty_var_return(t_token **head, int i)
{
	t_token	*new_token;

	new_token = create_token("$", TOKEN_WORD, QUOTE_NONE);
	if (!new_token)
		return (-1);
	add_token(head, new_token);
	return (i);
}

static int	handle_exit_status_return(t_token **head, t_env *env, int start)
{
	char	*value;
	t_token	*new_token;

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

static int	handle_pid_return(t_token **head, int start)
{
	char	*value;
	t_token	*new_token;

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

static int	handle_env_var_return(char *input, t_token **head, int start,
		t_env *env)
{
	int		i;
	char	*value;
	char	*env_value;
	t_token	*new_token;

	i = start;
	while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
		i++;
	if (i == start)
		return (handle_empty_var_return(head, start - 1));
	value = ft_substr(input, start, i - start);
	if (!value)
		return (-1);
	env_value = get_env_value(env, value);
	free(value);
	if (!env_value)
		new_token = create_token("", TOKEN_WORD, QUOTE_NONE);
	else
		new_token = create_token(env_value, TOKEN_WORD, QUOTE_NONE);
	if (!new_token)
		return (-1);
	add_token(head, new_token);
	return (i - 1);
}

int	token_handle_variable(char *input, int i, t_token **head, t_env *env)
{
	int	start;

	if (!input || !head || input[i] != '$')
		return (-1);
	start = i + 1;
	if (!input[start] || ft_isspace(input[start]))
		return (handle_empty_var_return(head, i));
	if (input[start] == '?')
		return (handle_exit_status_return(head, env, start));
	if (input[start] == '$')
		return (handle_pid_return(head, start));
	return (handle_env_var_return(input, head, start, env));
}
