/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 17:39:32 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/12 18:12:12 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token_type	get_operator_type(char *str)
{
	if (str[0] == '|')
		return (TOKEN_PIPE);
	else if (str[0] == '<')
	{
		if (str[1] == '<')
			return (TOKEN_HEREDOC);
		return (TOKEN_REDIR_IN);
	}
	else if (str[0] == '>')
	{
		if (str[1] == '>')
			return (TOKEN_REDIR_APPEND);
		return (TOKEN_REDIR_OUT);
	}
	return (TOKEN_OPERATOR);
}

t_token	*handle_operator(char **input)
{
	t_token_type	type;
	char			*value;
	int				len;

	type = get_operator_type(*input);
	len = 1;
	if (type == TOKEN_HEREDOC || type == TOKEN_REDIR_APPEND)
		len = 2;
	value = ft_substr(*input, 0, len);
	if (!value)
		return (NULL);
	*input += len;
	return (create_token(type, value));
}

t_token	*handle_syntax_check(t_token *head)
{
	if (!check_syntax_errors(head))
	{
		free_tokens(head);
		return (NULL);
	}
	return (head);
}

int	is_export_command(t_token *head)
{
	return (head && head->value && ft_strcmp(head->value, "export") == 0);
}
