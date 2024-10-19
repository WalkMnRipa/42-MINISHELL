/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 15:27:34 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/19 19:44:45 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

char	*expand_special_variable(t_env *env, const char *var_name)
{
	if (ft_strcmp(var_name, "?") == 0)
		return (ft_itoa(env->last_exit_status));
	else if (ft_strcmp(var_name, "$") == 0)
		return (ft_itoa(getpid()));
	return (NULL);
}

char	*get_env_variable(t_env *env, const char *var_name)
{
	char	*value;

	value = get_env_value(env, var_name);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

static void	update_quote_states(char c, int *in_single_quote,
		int *in_double_quote)
{
	if (c == '\'' && !(*in_double_quote))
		*in_single_quote = !(*in_single_quote);
	else if (c == '"' && !(*in_single_quote))
		*in_double_quote = !(*in_double_quote);
}

static char	*process_variable(char *str, int *i, t_env *env,
		int in_single_quote)
{
	if (!in_single_quote && str[*i] == '$' && str[*i + 1]
		&& is_valid_var_start(str[*i + 1]))
		return (replace_var(str, i, env));
	return (str);
}

char	*expand_variables(char *str, t_env *env)
{
	int	i;
	int	in_single_quote;
	int	in_double_quote;

	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (str[i])
	{
		update_quote_states(str[i], &in_single_quote, &in_double_quote);
		str = process_variable(str, &i, env, in_single_quote);
		i++;
	}
	return (str);
}