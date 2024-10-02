/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expansions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 20:02:25 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/01 20:02:42 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

static char	*get_env_value(t_shell *shell, const char *var_name)
{
	int		i;
	size_t	len;

	len = ft_strlen(var_name);
	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], var_name, len) == 0
			&& shell->env[i][len] == '=')
			return (shell->env[i] + len + 1);
		i++;
	}
	return (NULL);
}

char	*expand_env_vars(t_shell *shell, char *str)
{
	char	*result;
	char	*tmp;
	char	*var_start;
	char	*var_end;
	char	*var_name;
	char	*var_value;

	result = ft_strdup(str);
	while ((var_start = ft_strchr(result, '$')))
	{
		var_end = var_start + 1;
		while (*var_end && (ft_isalnum(*var_end) || *var_end == '_'))
			var_end++;
		var_name = ft_substr(var_start + 1, 0, var_end - var_start - 1);
		var_value = get_env_value(shell, var_name);
		if (var_value)
		{
			tmp = result;
			result = ft_strjoin_shell(ft_substr(result, 0, var_start - result),
					ft_strjoin_shell(var_value, var_end));
			free(tmp);
		}
		else
		{
			tmp = result;
			result = ft_strjoin_shell(ft_substr(result, 0, var_start - result),
					var_end);
			free(tmp);
		}
		free(var_name);
	}
	return (result);
}
