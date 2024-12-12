/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:57:57 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/12 17:29:05 by ggaribot         ###   ########.fr       */
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

static t_token	*split_expanded_word(char *word)
{
	char	**split_words;
	t_token	*head;
	t_token	*new_token;
	int		i;

	split_words = ft_split(word, ' ');
	if (!split_words)
		return (create_token(TOKEN_WORD, ft_strdup(word)));
	if (!split_words[0])
	{
		free_string_array(split_words, -1);
		return (create_token(TOKEN_WORD, ft_strdup(word)));
	}
	head = create_token(TOKEN_WORD, ft_strdup(split_words[0]));
	if (!head)
	{
		free_string_array(split_words, -1);
		return (NULL);
	}
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
	free_string_array(split_words, -1);
	return (head);
}

static int	should_split_word(const char *word, t_token *head,
		t_token *prev_token)
{
	// Don't split in export command
	if (is_export_command(head))
		return (0);
	// Don't split if it's part of an echo command
	if (prev_token && ft_strcmp(prev_token->value, "echo") == 0)
		return (0);
	// Don't split if original word had quotes (file names with spaces)
	if (ft_strchr(word, '"') || ft_strchr(word, '\''))
		return (0);
	// Split only for command execution (expanded variables)
	// Check if the word is an expanded variable result with spaces
	if (word && ft_strchr(word, ' ') && word[0] != '"' && word[0] != '\'')
	{
		// Only split if it's not part of a redirection
		if (prev_token && (prev_token->type == TOKEN_REDIR_IN
				|| prev_token->type == TOKEN_REDIR_OUT
				|| prev_token->type == TOKEN_REDIR_APPEND
				|| prev_token->type == TOKEN_HEREDOC))
			return (0);
		return (1);
	}
	return (0);
}

t_token	*tokenizer(char *input, t_env *env)
{
	t_token	*head;
	t_token	*new_token;
	t_token	*current;
	char	*word_value;

	head = NULL;
	current = head;
	while (*input)
	{
		new_token = get_next_token(&input, env);
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
		if (should_split_word(new_token->value, head, current))
		{
			word_value = ft_strdup(new_token->value);
			free_token(new_token);
			if (!word_value)
				return (NULL);
			new_token = split_expanded_word(word_value);
			free(word_value);
			if (!new_token)
				return (NULL);
		}
		if (!head)
			head = new_token;
		else
			add_token(&head, new_token);
		current = new_token;
	}
	return (handle_syntax_check(head));
}
