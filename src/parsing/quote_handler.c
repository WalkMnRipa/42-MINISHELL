/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:58:27 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/25 17:29:20 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

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

static void	handle_quote_removal(t_quote_data *data)
{
	if (data->state == STATE_NORMAL)
	{
		if (data->str[data->i] == '\'')
			data->state = STATE_SINGLE_QUOTE;
		else
			data->state = STATE_DOUBLE_QUOTE;
		data->current_quote = data->str[data->i];
	}
	else
		data->state = STATE_NORMAL;
}

char	*copy_without_quotes(char *str)
{
	t_quote_data	data;

	data.result = malloc(ft_strlen(str) + 1);
	if (!data.result)
		return (NULL);
	data.i = 0;
	data.j = 0;
	data.state = STATE_NORMAL;
	data.current_quote = 0;
	data.str = str;
	while (str[data.i])
	{
		if ((str[data.i] == '\'' || str[data.i] == '"')
			&& (data.state == STATE_NORMAL
				|| str[data.i] == data.current_quote))
		{
			handle_quote_removal(&data);
			data.i++;
			continue ;
		}
		data.result[data.j++] = str[data.i++];
	}
	data.result[data.j] = '\0';
	free(str);
	return (data.result);
}

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}
