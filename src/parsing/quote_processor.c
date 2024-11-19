/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_processor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 00:47:24 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/19 14:33:54 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

void	init_quote_processor(t_quote_processor *qp, char *input)
{
	qp->input = input;
	qp->result = NULL;
	qp->i = 0;
	qp->in_quotes = 0;
	qp->quote_char = 0;
}

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}
// if = starting quote
// else if = ending quote
static int	handle_quote_state(t_quote_processor *qp, char current_char)
{
	if (!qp->in_quotes && is_quote(current_char))
	{
		qp->in_quotes = 1;
		qp->quote_char = current_char;
		return (1);
	}
	else if (qp->in_quotes && current_char == qp->quote_char)
	{
		qp->in_quotes = 0;
		qp->quote_char = 0;
		return (1);
	}
	return (0);
}

static char	*append_char(char *str, char c)
{
	char	*new_str;
	char	tmp[2];

	tmp[0] = c;
	tmp[1] = '\0';
	if (!str)
		return (ft_strdup(tmp));
	new_str = ft_strjoin(str, tmp);
	free(str);
	return (new_str);
}

char	*process_quotes(char *input)
{
	t_quote_processor	qp;
	char				*new_str;

	if (!input)
		return (NULL);
	init_quote_processor(&qp, input);
	new_str = ft_strdup("");
	if (!new_str)
		return (NULL);
	while (input[qp.i])
	{
		if (handle_quote_state(&qp, input[qp.i]))
		{
			qp.i++;
			continue ;
		}
		new_str = append_char(new_str, input[qp.i]);
		if (!new_str)
			return (NULL);
		qp.i++;
	}
	if (qp.in_quotes)
	{
		free(new_str);
		return (NULL);
	}
	return (new_str);
}
