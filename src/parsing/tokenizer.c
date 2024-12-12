/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:57:57 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/12 17:54:12 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_export_command(t_token *head)
{
	return (head && head->value && ft_strcmp(head->value, "export") == 0);
}

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

static t_token	*handle_split_word_array(char **split_words)
{
	t_token	*head;
	t_token	*new_token;
	int		i;

	head = create_token(TOKEN_WORD, ft_strdup(split_words[0]));
	if (!head)
		return (free_string_array(split_words, -1), NULL);
	i = 1;
	while (split_words[i])
	{
		new_token = create_token(TOKEN_WORD, ft_strdup(split_words[i]));
		if (!new_token)
		{
			free_tokens(head);
			free_string_array(split_words, -1);
			return (NULL);
		}
		add_token(&head, new_token);
		i++;
	}
	return (free_string_array(split_words, -1), head);
}

static t_token	*split_expanded_word(char *word)
{
	char	**split_words;
	t_token	*head;

	split_words = ft_split(word, ' ');
	if (!split_words)
		return (create_token(TOKEN_WORD, ft_strdup(word)));
	if (!split_words[0])
	{
		free_string_array(split_words, -1);
		return (create_token(TOKEN_WORD, ft_strdup(word)));
	}
	head = handle_split_word_array(split_words);
	return (head);
}

static int	should_split_word(const char *word, t_token *head,
		t_token *prev_token)
{
	if (is_export_command(head))
		return (0);
	if (prev_token && ft_strcmp(prev_token->value, "echo") == 0)
		return (0);
	if (ft_strchr(word, '"') || ft_strchr(word, '\''))
		return (0);
	if (word && ft_strchr(word, ' ') && word[0] != '"' && word[0] != '\'')
	{
		if (prev_token && (prev_token->type == TOKEN_REDIR_IN
				|| prev_token->type == TOKEN_REDIR_OUT
				|| prev_token->type == TOKEN_REDIR_APPEND
				|| prev_token->type == TOKEN_HEREDOC))
			return (0);
		return (1);
	}
	return (0);
}

static t_token	*handle_word_splitting(t_token *token, char **word_value)
{
	t_token	*new_token;

	*word_value = ft_strdup(token->value);
	free_token(token);
	if (!*word_value)
		return (NULL);
	new_token = split_expanded_word(*word_value);
	free(*word_value);
	return (new_token);
}

static t_token	*process_token(char **input, t_env *env, t_token *head,
		t_token *current)
{
	t_token	*new_token;
	char	*word_value;

	new_token = get_next_token(input, env);
	if (!new_token)
		return (free_tokens(head), NULL);
	if (new_token->type == TOKEN_EOF)
		return (free_token(new_token), head);
	if (should_split_word(new_token->value, head, current))
		new_token = handle_word_splitting(new_token, &word_value);
	return (new_token);
}

static void	update_token_list(t_token **head, t_token **current,
		t_token *new_token)
{
	if (!*head)
		*head = new_token;
	else
		add_token(head, new_token);
	*current = new_token;
}

t_token	*tokenizer(char *input, t_env *env)
{
	t_token	*head;
	t_token	*new_token;
	t_token	*current;

	head = NULL;
	current = head;
	while (*input)
	{
		new_token = process_token(&input, env, head, current);
		if (!new_token && head)
			return (NULL);
		if (!new_token)
			break ;
		update_token_list(&head, &current, new_token);
	}
	return (handle_syntax_check(head));
}
