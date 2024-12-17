/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler_utils2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 19:50:31 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/17 19:56:10 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	init_quote_data(t_quote_data *data, char *str)
{
	data->i = 0;
	data->j = 0;
	data->state = STATE_NORMAL;
	data->current_quote = 0;
	data->str = str;
	data->result = malloc(ft_strlen(str) + 1);
}

static char	*handle_malloc_error(char *str)
{
	cleanup_ptr(str);
	return (NULL);
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

static void	copy_chars(t_quote_data *data)
{
	while (data->str[data->i])
	{
		if ((data->str[data->i] == '\'' || data->str[data->i] == '"')
			&& (data->state == STATE_NORMAL
				|| data->str[data->i] == data->current_quote))
		{
			handle_quote_removal(data);
			data->i++;
			continue ;
		}
		data->result[data->j++] = data->str[data->i++];
	}
	data->result[data->j] = '\0';
}

char	*copy_without_quotes(char *str)
{
	t_quote_data	data;

	if (!str)
		return (NULL);
	init_quote_data(&data, str);
	if (!data.result)
		return (handle_malloc_error(str));
	copy_chars(&data);
	cleanup_ptr(str);
	return (data.result);
}
