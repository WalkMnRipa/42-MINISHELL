/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 17:07:48 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/08 17:40:47 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

static char	*get_var_value(char *var_name, t_env *env, int last_exit_status)
{
	if (ft_strcmp(var_name, "?") == 0)
		return (ft_itoa(last_exit_status));
	return (getenv(var_name));
}

char	*expand_variables(char *str, t_env *env, int last_exit_status)
{
	char	*result;
	char	*var_name;
	char	*var_value;
	int		i;
	int		j;

	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] && !ft_isspace(str[i + 1]))
		{
			j = ++i;
			while (str[j] && (ft_isalnum(str[j]) || str[j] == '_'))
				j++;
			var_name = ft_substr(str, i, j - i);
			var_value = get_var_value(var_name, env, last_exit_status);
			result = ft_strjoin_free(result, var_value);
			free(var_name);
			i = j;
		}
		else
			result = ft_strjoin_char(result, str[i++]);
	}
	return (result);
}

void	expand_variables_in_tokens(t_token **head, t_env *env,
		int last_exit_status)
{
	t_token	*current;
	char	*expanded;

	current = *head;
	while (current)
	{
		if (current->type == TOKEN_WORD && current->quote_type != QUOTE_SINGLE)
		{
			expanded = expand_variables(current->value, env, last_exit_status);
			free(current->value);
			current->value = expanded;
		}
		current = current->next;
	}
}
