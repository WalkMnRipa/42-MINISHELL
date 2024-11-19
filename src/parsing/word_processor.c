/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_processor.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 00:49:12 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/19 00:49:14 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

void	init_word_processor(t_word_processor *wp, char *input, t_token **head,
		t_env *env)
{
	wp->input = input;
	wp->head = head;
	wp->env = env;
	wp->i = 0;
	wp->start = 0;
}

int	is_word_char(char c)
{
	return (!is_operator(c) && !ft_isspace(c));
}

char	*get_word(char *input, int start, int end)
{
	if (start >= end)
		return (NULL);
	return (ft_substr(input, start, end - start));
}

static int	create_word_token(t_word_processor *wp, int end)
{
	char	*word;
	t_token	*token;

	word = get_word(wp->input, wp->start, end);
	if (!word)
		return (-1);
	if (*word) // Only create token if word is not empty
	{
		token = create_token(word, TOKEN_WORD, QUOTE_NONE);
		free(word);
		if (!token)
			return (-1);
		add_token(wp->head, token);
	}
	else
		free(word);
	wp->start = end;
	return (0);
}

int	process_words(t_process_state *ps)
{
	t_word_processor	wp;

	init_word_processor(&wp, ps->processed_input, ps->head, ps->env);
	while (wp.input[wp.i])
	{
		if (!is_word_char(wp.input[wp.i]))
		{
			if (create_word_token(&wp, wp.i) < 0)
				return (-1);
			wp.i++;
			wp.start = wp.i;
			continue ;
		}
		wp.i++;
	}
	// Handle last word if exists
	if (wp.start < wp.i)
		if (create_word_token(&wp, wp.i) < 0)
			return (-1);
	return (0);
}
