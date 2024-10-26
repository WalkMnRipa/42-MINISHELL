/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 00:37:28 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/26 16:11:42 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

static char	*handle_special_var(t_env *env, char special_char)
{
	if (special_char == '?')
		return (ft_itoa(env->last_exit_status));
	return (ft_itoa(getpid()));
}

static char	*join_and_free(char *s1, char *s2, int free_s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	if (free_s2)
		free(s2);
	return (result);
}

static int	expand_char(char **result, char c)
{
	char	*new_result;

	new_result = ft_strjoinc(*result, c);
	if (!new_result)
		return (1);
	free(*result);
	*result = new_result;
	return (0);
}

static int	handle_var_expansion(char **result, const char *str, int *i,
		t_env *env)
{
	char	*tmp;
	char	*var_name;
	char	*env_val;
	int		start;

	(*i)++;
	if (!str[*i] || ft_isspace(str[*i]))
	{
		*result = join_and_free(*result, "$", 0);
		(*i)--;
		return (*result == NULL);
	}
	if (str[*i] == '?' || str[*i] == '$')
	{
		tmp = handle_special_var(env, str[*i]);
		*result = join_and_free(*result, tmp, 1);
		return (*result == NULL);
	}
	start = *i;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	if (start == *i)
		return (*result = join_and_free(*result, "$", 0), *result == NULL);
	var_name = ft_substr(str, start, *i - start);
	env_val = get_env_value(env, var_name);
	free(var_name);
	if (env_val)
		*result = join_and_free(*result, env_val, 0);
	(*i)--;
	return (*result == NULL);
}

char	*expand_variables_in_str(char *str, t_env *env, t_quote_type quote_type)
{
	char	*result;
	int		i;

	if (!str)
		return (NULL);
	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = -1;
	while (str[++i])
	{
		if (str[i] == '$' && quote_type != QUOTE_SINGLE)
		{
			if (handle_var_expansion(&result, str, &i, env))
				return (free(result), NULL);
			continue ;
		}
		if (expand_char(&result, str[i]))
			return (free(result), NULL);
	}
	return (result);
}
