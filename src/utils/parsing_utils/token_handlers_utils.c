/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handlers_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 19:26:44 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/23 19:33:31 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parsing.h"

char	*get_quoted_content(char *input, int start, char quote)
{
	int	end;

	end = start;
	while (input[end] && input[end] != quote)
		end++;
	if (!input[end])
		return (NULL);
	return (ft_substr(input, start, end - start));
}

int	get_quote_end(char *input, int start, char quote)
{
	int	end;

	end = start;
	while (input[end] && input[end] != quote)
		end++;
	return (end);
}

static char	*process_quoted_segment(char *word, int *i, t_env *env)
{
	char	quote;
	int		start;
	char	*segment;
	char	*expanded;

	quote = word[*i];
	start = ++(*i);
	while (word[*i] && word[*i] != quote)
		(*i)++;
	segment = ft_substr(word, start, *i - start);
	if (!segment)
		return (NULL);
	if (quote == '"')
		expanded = expand_variables_in_str(segment, env, QUOTE_DOUBLE);
	else
		expanded = ft_strdup(segment);
	free(segment);
	return (expanded);
}

static char	*process_unquoted_segment(char *word, int *i, t_env *env)
{
	int		start;
	char	*segment;

	start = *i;
	while (word[*i] && word[*i] != '\'' && word[*i] != '"')
		(*i)++;
	segment = ft_substr(word, start, *i - start);
	if (!segment)
		return (NULL);
	return (expand_variables_in_str(segment, env, QUOTE_NONE));
}

char	*expand_quoted_word(char *word, t_env *env)
{
	char	*result;
	char	*segment;
	char	*temp;
	int		i;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (word[i])
	{
		if (word[i] == '\'' || word[i] == '"')
			segment = process_quoted_segment(word, &i, env);
		else
			segment = process_unquoted_segment(word, &i, env);
		if (!segment)
			return (free(result), NULL);
		temp = ft_strjoin(result, segment);
		free(result);
		free(segment);
		result = temp;
		i += (word[i] != '\0');
	}
	return (result);
}
