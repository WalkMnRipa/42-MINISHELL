/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 00:37:28 by jcohen            #+#    #+#             */
/*   Updated: 2024/11/18 23:41:36 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

static char	*handle_special_var(t_env *env, char special_char)
{
	if (special_char == '?')
		return (ft_itoa(env->last_exit_status));
	return (ft_itoa(getpid()));
}

char	*expand_variables_in_str(char *str, t_env *env, t_quote_type quote_type)
{
	char	*result;
	char	*temp;
	char	*var_name;
	char	*var_value;
	int		i;
	int		j;
	char	*joined;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && quote_type != QUOTE_SINGLE)
		{
			i++;
			if (!str[i] || ft_isspace(str[i]))
			{
				temp = ft_strjoin(result, "$");
				free(result);
				result = temp;
				if (!result)
					return (NULL);
			}
			else if (str[i] == '?' || str[i] == '$')
			{
				temp = handle_special_var(env, str[i]);
				if (!temp)
					return (free(result), NULL);
				joined = ft_strjoin(result, temp);
				free(temp);
				free(result);
				result = joined;
				if (!result)
					return (NULL);
				i++;
			}
			else
			{
				j = i;
				while (str[j] && (ft_isalnum(str[j]) || str[j] == '_'))
					j++;
				var_name = ft_substr(str, i, j - i);
				if (!var_name)
					return (free(result), NULL);
				var_value = get_env_value(env, var_name);
				free(var_name);
				temp = ft_strjoin(result, var_value ? var_value : "");
				free(result);
				result = temp;
				if (!result)
					return (NULL);
				i = j;
				continue ;
			}
		}
		else
		{
			temp = ft_strjoinc(result, str[i]);
			free(result);
			result = temp;
			if (!result)
				return (NULL);
			i++;
		}
	}
	return (result);
}
