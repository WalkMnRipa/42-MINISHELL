/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handlers_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 19:26:44 by jcohen            #+#    #+#             */
/*   Updated: 2024/11/18 15:31:32 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parsing.h"

int	get_quote_end(char *input, int start, char quote)
{
	int	i;

	i = start;
	while (input[i] && input[i] != quote)
		i++;
	if (!input[i]) // No closing quote found
		return (-1);
	return (i);
}

void	init_quote_state(t_quote_state *state)
{
	state->current = QUOTE_NONE;
	state->previous = QUOTE_NONE;
	state->nested_level = 0;
}

int	is_quote_char(char c)
{
	return (c == '\'' || c == '"');
}

int	update_quote_state(t_quote_state *state, char quote_char)
{
	if (state->current == QUOTE_NONE)
	{
		state->previous = state->current;
		state->current = (quote_char == '\'') ? QUOTE_SINGLE : QUOTE_DOUBLE;
		state->nested_level++;
		return (1);
	}
	else if ((state->current == QUOTE_SINGLE && quote_char == '\'')
			|| (state->current == QUOTE_DOUBLE && quote_char == '"'))
	{
		state->previous = state->current;
		state->current = QUOTE_NONE;
		state->nested_level--;
		return (1);
	}
	return (0);
}

char	*get_quoted_content(char *input, int start, char quote)
{
	int		i;
	char	*content;

	i = start;
	while (input[i] && input[i] != quote)
		i++;
	if (!input[i]) // No closing quote found
		return (NULL);
	content = ft_substr(input, start, i - start);
	return (content);
}

char	*expand_quoted_word(char *word, t_env *env)
{
	char	*result;
	char	*temp;
	int		i;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (word[i])
	{
		if (word[i] == '$')
		{
			temp = result;
			result = expand_variables_in_str(word + i, env, QUOTE_NONE);
			free(temp);
			if (!result)
				return (NULL);
			while (word[i] && !ft_isspace(word[i]))
				i++;
			i--;
		}
		else
		{
			temp = ft_strjoinc(result, word[i]);
			free(result);
			result = temp;
			if (!result)
				return (NULL);
		}
		i++;
	}
	return (result);
}
