/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator_processor.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 00:48:58 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/19 00:49:00 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

void	init_operator_processor(t_operator_processor *op, char *input,
		t_token **head)
{
	op->input = input;
	op->head = head;
	op->i = 0;
}

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static t_token_type	get_operator_type(char *str, int len)
{
	if (len == 1)
	{
		if (str[0] == '|')
			return (TOKEN_PIPE);
		if (str[0] == '<')
			return (TOKEN_REDIR_INPUT);
		if (str[0] == '>')
			return (TOKEN_REDIR_OUTPUT);
	}
	else if (len == 2)
	{
		if (str[0] == '<' && str[1] == '<')
			return (TOKEN_HERE_DOC);
		if (str[0] == '>' && str[1] == '>')
			return (TOKEN_REDIR_APPEND);
	}
	return (TOKEN_ERROR);
}

int	get_operator_length(char *str, int start)
{
	if (!str[start] || !is_operator(str[start]))
		return (0);
	if (str[start + 1] && str[start] == str[start + 1] && (str[start] == '<'
			|| str[start] == '>'))
		return (2);
	return (1);
}

static int	create_operator_token(t_operator_processor *op, int len)
{
	char			*value;
	t_token			*token;
	t_token_type	type;

	value = ft_substr(op->input, op->i, len);
	if (!value)
		return (-1);
	type = get_operator_type(value, len);
	if (type == TOKEN_ERROR)
	{
		free(value);
		return (-1);
	}
	token = create_token(value, type, QUOTE_NONE);
	free(value);
	if (!token)
		return (-1);
	add_token(op->head, token);
	op->i += len - 1;
	return (0);
}

int	process_operators(t_process_state *ps)
{
	t_operator_processor	op;
	int						len;

	init_operator_processor(&op, ps->processed_input, ps->head);
	while (op.input[op.i])
	{
		if (is_operator(op.input[op.i]))
		{
			len = get_operator_length(op.input, op.i);
			if (len == 0 || create_operator_token(&op, len) < 0)
				return (-1);
		}
		op.i++;
	}
	return (0);
}
