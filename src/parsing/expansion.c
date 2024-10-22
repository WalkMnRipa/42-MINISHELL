/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:29:35 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/22 18:07:09 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

static char	*expand_special_var(char specifier, t_env *env)
{
	if (specifier == '?')
		return (ft_itoa(env->last_exit_status));
	if (specifier == '$')
		return (ft_itoa(getpid()));
	return (ft_strdup("$"));
}

static char	*expand_env_var(char *str, int *i, t_env *env)
{
	int		start;
	char	*var_name;
	char	*value;
	char	*var_value;

	start = *i;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	var_name = ft_substr(str, start, *i - start);
	if (!var_name)
		return (NULL);
	value = get_env_value(env, var_name);
	var_value = value ? ft_strdup(value) : ft_strdup("");
	free(var_name);
	(*i)--;
	return (var_value);
}

static char	*add_char_to_result(char *result, char c)
{
	char	str[2];
	char	*new_result;

	str[0] = c;
	str[1] = '\0';
	new_result = ft_strjoin(result, str);
	free(result);
	return (new_result);
}

char	*expand_variables_in_str(char *str, t_env *env, t_quote_type quote_type)
{
	char	*result;
	char	*temp;
	int		i;
	char	*new_result;

	if (!str || quote_type == QUOTE_SINGLE)
		return (ft_strdup(str));
	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
		{
			i++;
			if (str[i] == '?' || str[i] == '$')
			{
				temp = expand_special_var(str[i], env);
				i++;
			}
			else
				temp = expand_env_var(str, &i, env);
			if (!temp)
				return (free(result), NULL);
			new_result = ft_strjoin(result, temp);
			free(temp);
			free(result);
			result = new_result;
		}
		else
		{
			result = add_char_to_result(result, str[i]);
			if (!result)
				return (NULL);
			i++;
		}
	}
	return (result);
}
