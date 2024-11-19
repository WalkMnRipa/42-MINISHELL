/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 00:59:39 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/19 00:59:46 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

static char	*get_var_content(t_env *env, const char *var_name)
{
	if (!var_name)
		return (NULL);
	if (ft_strcmp(var_name, "?") == 0)
		return (ft_itoa(env->last_exit_status));
	return (ft_strdup(get_env_value(env, var_name)));
}

static char	*handle_dollar(char *str, int *i, t_env *env,
		t_quote_type quote_type)
{
	char	*var_name;
	char	*var_content;
	int		j;

	if (str[*i + 1] == '?')
	{
		(*i)++;
		return (ft_itoa(env->last_exit_status));
	}
	j = *i + 1;
	while (str[j] && (ft_isalnum(str[j]) || str[j] == '_'))
		j++;
	var_name = ft_substr(str, *i + 1, j - (*i + 1));
	if (!var_name)
		return (NULL);
	var_content = get_var_content(env, var_name);
	free(var_name);
	*i = j - 1;
	if (!var_content && quote_type == QUOTE_NONE)
		return (ft_strdup(""));
	return (var_content);
}

char	*expand_variables_in_str(char *str, t_env *env, t_quote_type quote_type)
{
	char	*result;
	char	*tmp;
	char	*var_content;
	int		i;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && (quote_type != QUOTE_SINGLE))
		{
			var_content = handle_dollar(str, &i, env, quote_type);
			tmp = result;
			result = ft_strjoin(result, var_content);
			free(tmp);
			free(var_content);
			if (!result)
				return (NULL);
		}
		else
		{
			tmp = result;
			result = ft_strjoinc(result, str[i]);
			free(tmp);
			if (!result)
				return (NULL);
		}
		i++;
	}
	return (result);
}
