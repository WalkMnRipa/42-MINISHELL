/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 17:03:21 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/16 23:33:02 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*process_quotes(char *result, int *i, t_quote_state *quote_state)
{
	*quote_state = get_quote_state(result[*i], *quote_state);
	(*i)++;
	return (result);
}

static char	*process_string(char *result, t_env *env, t_quote_state quote_state)
{
	int	i;

	i = 0;
	while (result[i])
	{
		if (result[i] == '\'' || result[i] == '"')
			result = process_quotes(result, &i, &quote_state);
		else if (result[i] == '$' && quote_state != STATE_SINGLE_QUOTE)
		{
			result = expand_variables_in_str(result, env, quote_state);
			if (!result)
				return (NULL);
			i++;
		}
		else
			i++;
	}
	return (result);
}

char	*handle_quotes(char *str, t_env *env)
{
	t_quote_state	quote_state;
	char			*result;

	if (!str)
		return (NULL);
	result = ft_strdup(str);
	if (!result)
		return (NULL);
	quote_state = STATE_NORMAL;
	result = process_string(result, env, quote_state);
	if (!result)
		return (NULL);
	return (copy_without_quotes(result));
}

int	is_quote_closed(char *str)
{
	t_quote_state	state;
	int				i;

	state = STATE_NORMAL;
	i = 0;
	while (str[i])
	{
		if (is_quote(str[i]))
			state = get_quote_state(str[i], state);
		i++;
	}
	return (state == STATE_NORMAL);
}
