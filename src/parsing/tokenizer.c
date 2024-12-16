/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:57:57 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/16 23:33:07 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	get_word_length(char *input, t_quote_state *state)
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

static char	*extract_word(char **input, t_env *env)
{
	char			*start;
	int				len;
	char			*word;
	char			*processed_word;
	t_quote_state	quote_state;

	start = *input;
	quote_state = STATE_NORMAL;
	len = get_word_length(*input, &quote_state);
	word = ft_substr(start, 0, len);
	if (!word)
		return (NULL);
	processed_word = handle_quotes(word, env);
	if (!processed_word)
	{
		free(word);
		return (NULL);
	}
	*input += len;
	free(word);
	return (processed_word);
}

t_token	*get_next_token(char **input, t_env *env)
{
	t_token	*token;
	char	*value;

	while (**input && is_whitespace(**input))
		(*input)++;
	if (!**input)
		return (create_token(TOKEN_EOF, NULL));
	if (is_operator(**input))
		return (handle_operator(input));
	value = extract_word(input, env);
	if (!value)
		return (NULL);
	token = create_token(TOKEN_WORD, value);
	if (!token)
	{
		free(value);
		return (NULL);
	}
	return (token);
}

t_token	*tokenizer(char *input, t_env *env)
{
	t_token	*head;
	t_token	*new_token;
	char	*current;

	head = NULL;
	current = input;
	while (*current)
	{
		new_token = get_next_token(&current, env);
		if (!new_token)
		{
			if (head)
				free_tokens(head);
			return (NULL);
		}
		if (new_token->type == TOKEN_EOF)
		{
			free_token(new_token);
			break ;
		}
		add_token(&head, new_token);
	}
	return (handle_syntax_check(head));
}
