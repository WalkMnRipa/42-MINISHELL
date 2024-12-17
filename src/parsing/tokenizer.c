/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:57:57 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/17 21:14:41 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	cleanup_ptr(word);
	if (!processed_word)
		return (NULL);
	*input += len;
	return (processed_word);
}

static t_token	*create_words_from_args(char **args)
{
	t_token	*head;
	t_token	*current;
	t_token	*tmp;
	int		i;

	head = NULL;
	i = 0;
	while (args[i])
	{
		current = create_token(TOKEN_WORD, ft_strdup(args[i]));
		if (!current)
			return (free_tokens(head), NULL);
		if (!head)
			head = current;
		else
		{
			tmp = head;
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = current;
		}
		i++;
	}
	return (head);
}

t_token	*handle_expanded_command(char *value)
{
	t_token	*head;
	char	**args;

	if (!ft_strchr(value, '\x1F'))
		return (create_token(TOKEN_WORD, value));
	args = ft_split(value, '\x1F');
	cleanup_ptr(value);
	if (!args)
		return (NULL);
	head = create_words_from_args(args);
	free_string_array(args, -1);
	return (head);
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
	if (value[0] != '\0' && ft_strchr(value, '\x1F'))
		return (handle_expanded_command(value));
	token = create_token(TOKEN_WORD, value);
	if (!token)
	{
		cleanup_ptr(value);
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
