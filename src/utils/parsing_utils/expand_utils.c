/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 19:31:10 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/19 19:45:03 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parsing.h"

char	*get_var_value(t_env *env, const char *var_name)
{
	char	*expanded;

	expanded = expand_special_variable(env, var_name);
	if (expanded)
		return (expanded);
	return (get_env_variable(env, var_name));
}

char	*replace_var(char *str, int *i, t_env *env)
{
	int		start;
	char	*var_name;
	char	*var_value;
	char	*result;
	char	*before;

	start = *i + 1;
	(*i)++;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	var_name = ft_substr(str, start, *i - start);
	var_value = get_var_value(env, var_name);
	free(var_name);
	before = ft_substr(str, 0, start - 1);
	result = ft_strjoin_free(before, var_value);
	result = ft_strjoin_free(result, str + *i);
	free(str);
	*i = ft_strlen(result) - ft_strlen(str + *i) - 1;
	return (result);
}
