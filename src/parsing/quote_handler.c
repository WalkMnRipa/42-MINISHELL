/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:58:27 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/17 19:51:54 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_quote_state	get_quote_state(char c, t_quote_state current)
{
	if (current == STATE_NORMAL)
	{
		if (c == '\'')
			return (STATE_SINGLE_QUOTE);
		if (c == '"')
			return (STATE_DOUBLE_QUOTE);
	}
	else if (current == STATE_SINGLE_QUOTE && c == '\'')
		return (STATE_NORMAL);
	else if (current == STATE_DOUBLE_QUOTE && c == '"')
		return (STATE_NORMAL);
	return (current);
}

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

int	is_quote_closed(char *str)
{
	t_quote_state	state;
	int				i;

	if (!str)
		return (1);
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
