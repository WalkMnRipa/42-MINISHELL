/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 17:39:32 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/17 01:27:07 by ggaribot         ###   ########.fr       */
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

int	get_word_length(char *input, t_quote_state *state)
{
	int	len;

	len = 0;
	while (input[len])
	{
		if (is_quote(input[len]))
			*state = get_quote_state(input[len], *state);
		if (*state == STATE_NORMAL && !is_quote(input[len]))
		{
			if (is_whitespace(input[len]) || (is_operator(input[len])
					&& len == 0))
				break ;
		}
		len++;
	}
	return (len);
}

char	*get_var_value(char *var_name, t_env *env)
{
	char	*env_val;

	if (ft_strcmp(var_name, "?") == 0)
		return (ft_itoa(env->last_exit_status));
	env_val = get_env_value(env, var_name);
	if (env_val)
		return (ft_strdup(env_val));
	return (ft_strdup(""));
}
