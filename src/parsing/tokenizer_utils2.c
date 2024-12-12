/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 18:03:20 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/12 18:05:48 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*handle_split_word_array(char **split_words)
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

t_token	*split_expanded_word(char *word)
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

int	should_split_word(const char *word, t_token *head, t_token *prev_token)
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

t_token	*handle_word_splitting(t_token *token)
{
	t_token	*new_token;
	char	*word_value;

	word_value = ft_strdup(token->value);
	free_token(token);
	if (!word_value)
		return (NULL);
	new_token = split_expanded_word(word_value);
	free(word_value);
	return (new_token);
}

t_token	*process_tokenizer_token(char **input, t_env *env, t_token *head,
		t_token *current)
{
	t_token	*new_token;

	new_token = get_next_token(input, env);
	if (!new_token)
		return (free_tokens(head), NULL);
	if (new_token->type == TOKEN_EOF)
		return (free_token(new_token), head);
	if (should_split_word(new_token->value, head, current))
		new_token = handle_word_splitting(new_token);
	return (new_token);
}
