/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:59:44 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/25 18:12:08 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	should_skip_char(char c)
{
	if (!c || c == ' ' || c == '"' || c == '\'' || c == '\\' || c == '<'
		|| c == '>' || c == '|' || c == '&')
		return (1);
	return (0);
}

static int	is_valid_var_char(char c)
{
	if (ft_isalnum(c) || c == '_' || c == '?')
		return (1);
	return (0);
}

static char	*handle_var_expansion(char *str, int *i, t_env *env)
{
	if (should_skip_char(str[*i + 1]))
	{
		(*i)++;
		return (str);
	}
	if (is_valid_var_char(str[*i + 1]))
		return (expand_single_var(str, i, env));
	(*i)++;
	return (str);
}

char	*expand_variables_in_str(char *str, t_env *env, t_quote_state state)
{
	int		i;
	char	*result;

	if (!str)
		return (NULL);
	result = str;
	i = 0;
	while (result[i])
	{
		if (result[i] == '$' && state != STATE_SINGLE_QUOTE)
		{
			result = handle_var_expansion(result, &i, env);
			if (!result)
				return (NULL);
			continue ;
		}
		i++;
	}
	return (result);
}
