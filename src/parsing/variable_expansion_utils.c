/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 17:43:17 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/17 19:42:51 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*extract_var_name(char *str, int *i)
{
	int		s;
	int		len;
	char	*name;

	s = *i + 1;
	len = 0;
	if (str[s] == '?')
	{
		*i += 1;
		return (ft_strdup("?"));
	}
	while (str[s + len] && (ft_isalnum(str[s + len]) || str[s + len] == '_'))
		len++;
	if (len == 0)
		return (NULL);
	name = ft_substr(str, s, len);
	if (!name)
		return (NULL);
	*i += len;
	return (name);
}

static char	*process_expanded_command(char *expanded_value)
{
	char	*result;
	char	**args;
	int		i;
	char	*temp;

	if (!ft_strchr(expanded_value, ' '))
		return (expanded_value);
	args = ft_split(expanded_value, ' ');
	if (!args)
		return (expanded_value);
	result = ft_strdup(args[0]);
	if (!result)
		return (free_string_array(args, -1), cleanup_ptr(expanded_value), NULL);
	i = 1;
	while (args[i])
	{
		temp = result;
		result = ft_strjoin3(result, "\x1F", args[i]);
		cleanup_ptr(temp);
		if (!result)
			return (free_string_array(args, -1), cleanup_ptr(expanded_value),
				NULL);
		i++;
	}
	return (free_string_array(args, -1), cleanup_ptr(expanded_value), result);
}

char	*join_expanded_var(char *before, char *var_value, char *after, int *i)
{
	char	*temp;
	char	*result;

	temp = ft_strjoin(before, var_value);
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, after);
	cleanup_ptr(temp);
	if (!result)
		return (NULL);
	*i = ft_strlen(before) + ft_strlen(var_value) - 1;
	return (result);
}

static char	*handle_var_expansion(char *str, char *before, char *after,
		t_var_data *data)
{
	char	*var_value;
	char	*result;

	var_value = get_var_value(data->var_name, data->env);
	if (var_value)
		var_value = process_expanded_command(var_value);
	result = join_expanded_var(before, var_value, after, data->i);
	cleanup_ptr(data->var_name);
	cleanup_ptr(var_value);
	cleanup_ptr(before);
	cleanup_ptr(after);
	if (result)
		return (result);
	return (str);
}

char	*expand_single_var(char *str, int *i, t_env *env)
{
	char		*before;
	char		*after;
	t_var_data	data;

	data.var_name = extract_var_name(str, i);
	if (!data.var_name)
		return (str);
	before = ft_substr(str, 0, *i - ft_strlen(data.var_name));
	after = ft_strdup(str + *i + 1);
	if (!before || !after)
	{
		cleanup_ptr(data.var_name);
		if (before)
			cleanup_ptr(before);
		if (after)
			cleanup_ptr(after);
		return (str);
	}
	data.env = env;
	data.i = i;
	return (handle_var_expansion(str, before, after, &data));
}
