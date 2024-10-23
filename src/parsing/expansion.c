/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:29:35 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/23 16:32:30 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

char	*expand_variables_in_str(char *str, t_env *env, t_quote_type quote_type)
{
	char	*result;
	int		i;
	char	*var_name;
	char	*env_val;
	char	*new_result;
	char	*tmp;
	int		start;

	if (!str)
		return (NULL);
	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && quote_type != QUOTE_SINGLE)
		{
			i++;
			if (str[i] == '?' || str[i] == '$')
			{
				tmp = (str[i] == '?') ? ft_itoa(env->last_exit_status) : ft_itoa(getpid());
				if (!tmp)
				{
					free(result);
					return (NULL);
				}
				new_result = ft_strjoin(result, tmp);
				free(tmp);
				free(result);
				result = new_result;
				if (!result)
					return (NULL);
				i++;
				continue ;
			}
			start = i;
			while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
				i++;
			if (start == i)
			{
				new_result = ft_strjoinc(result, '$');
				free(result);
				result = new_result;
				if (!result)
					return (NULL);
				continue ;
			}
			var_name = ft_substr(str, start, i - start);
			if (!var_name)
			{
				free(result);
				return (NULL);
			}
			env_val = get_env_value(env, var_name);
			free(var_name);
			if (env_val)
			{
				new_result = ft_strjoin(result, env_val);
				free(result);
				result = new_result;
				if (!result)
					return (NULL);
			}
			i--;
		}
		else
		{
			new_result = ft_strjoinc(result, str[i]);
			free(result);
			result = new_result;
			if (!result)
				return (NULL);
		}
		i++;
	}
	return (result);
}
