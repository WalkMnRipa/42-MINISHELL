/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 17:43:17 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/25 17:52:34 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

static char	*extract_var_name(char *str, int *i)
{
	int		start;
	int		len;
	char	*name;

	start = *i + 1;
	len = 0;
	if (str[start] == '?')
	{
		*i += 1;
		return (ft_strdup("?"));
	}
	while (str[start + len] && (ft_isalnum(str[start + len]) || str[start
				+ len] == '_'))
		len++;
	if (len == 0)
		return (NULL);
	name = ft_substr(str, start, len);
	*i += len;
	return (name);
}

static void	free_expansion_data(char *var_name, char *var_value, char *before,
		char *after)
{
	free(var_name);
	if (var_value)
		free(var_value);
	if (before)
		free(before);
	if (after)
		free(after);
}

static char	*get_var_value(char *var_name, t_env *env)
{
	char	*env_val;

	if (ft_strcmp(var_name, "?") == 0)
		return (ft_itoa(env->last_exit_status));
	env_val = get_env_value(env, var_name);
	if (env_val)
		return (ft_strdup(env_val));
	return (ft_strdup(""));
}

char	*expand_single_var(char *str, int *i, t_env *env)
{
	char	*var_name;
	char	*var_value;
	char	*before;
	char	*after;
	char	*result;

	var_name = extract_var_name(str, i);
	if (!var_name)
		return (str);
	before = ft_substr(str, 0, *i - ft_strlen(var_name));
	after = ft_strdup(str + *i + 1);
	if (!before || !after)
	{
		free_expansion_data(var_name, NULL, before, after);
		return (str);
	}
	var_value = get_var_value(var_name, env);
	result = join_expanded_var(before, var_value, after, i);
	free_expansion_data(var_name, var_value, before, after);
	if (!result)
		return (str);
	free(str);
	return (result);
}

char	*join_expanded_var(char *before, char *var_value, char *after, int *i)
{
	char	*temp;
	char	*result;

	temp = ft_strjoin(before, var_value);
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, after);
	if (!result)
	{
		free(temp);
		return (NULL);
	}
	*i = ft_strlen(before) + ft_strlen(var_value) - 1;
	free(temp);
	return (result);
}
