/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:00:39 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/23 16:33:11 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parsing.h"

int	token_handle_single_quotes(char *input, int i, t_token **head)
{
	int		start;
	int		end;
	char	*value;
	t_token	*new_token;

	if (!input[i + 1])
		return (-1); // Quote non fermée
	start = i + 1;
	end = start;
	while (input[end] && input[end] != '\'')
		end++;
	if (!input[end]) // Quote non fermée
		return (-1);
	value = ft_substr(input, start, end - start);
	if (!value)
		return (-1);
	new_token = create_token(value, TOKEN_WORD, QUOTE_SINGLE);
	free(value);
	if (!new_token)
		return (-1);
	add_token(head, new_token);
	return (end);
}

int	token_handle_double_quotes(char *input, int i, t_token **head, t_env *env)
{
	int		start;
	int		end;
	char	*content;
	char	*expanded;
	t_token	*new_token;

	if (!input[i + 1])
		return (-1);
	start = i + 1;
	end = start;
	while (input[end] && input[end] != '"')
		end++;
	if (!input[end])
		return (-1);
	content = ft_substr(input, start, end - start);
	if (!content)
		return (-1);
	expanded = expand_variables_in_str(content, env, QUOTE_DOUBLE);
	free(content);
	if (!expanded)
		return (-1);
	new_token = create_token(expanded, TOKEN_WORD, QUOTE_DOUBLE);
	free(expanded);
	if (!new_token)
		return (-1);
	add_token(head, new_token);
	return (end);
}

char	*expand_quoted_word(char *word, t_env *env)
{
	char	*result;
	int		i;
	int		start;
	char	quote;
	char	*segment;
	char	*expanded;
	char	*temp;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (word[i])
	{
		if (word[i] == '\'' || word[i] == '"')
		{
			quote = word[i];
			start = i + 1;
			i++;
			while (word[i] && word[i] != quote)
				i++;
			segment = ft_substr(word, start, i - start);
			if (!segment)
			{
				free(result);
				return (NULL);
			}
			// Expansion selon le type de quote
			if (quote == '"')
				expanded = expand_variables_in_str(segment, env, QUOTE_DOUBLE);
			else
				expanded = ft_strdup(segment);
			// Pas d'expansion dans les quotes simples
			free(segment);
			if (!expanded)
			{
				free(result);
				return (NULL);
			}
			temp = ft_strjoin(result, expanded);
			free(expanded);
			free(result);
			if (!temp)
				return (NULL);
			result = temp;
		}
		else
		{
			start = i;
			while (word[i] && word[i] != '\'' && word[i] != '"')
				i++;
			segment = ft_substr(word, start, i - start);
			if (!segment)
			{
				free(result);
				return (NULL);
			}
			expanded = expand_variables_in_str(segment, env, QUOTE_NONE);
			free(segment);
			if (!expanded)
			{
				free(result);
				return (NULL);
			}
			temp = ft_strjoin(result, expanded);
			free(expanded);
			free(result);
			if (!temp)
				return (NULL);
			result = temp;
			continue ;
		}
		i++;
	}
	return (result);
}

int	token_handle_word(char *input, int i, t_token **head, t_env *env)
{
	int				start;
	int				current;
	char			*value;
	char			*expanded;
	t_token			*new_token;
	t_quote_type	quote_type;

	start = i;
	current = i;
	quote_type = QUOTE_NONE;
	while (input[current])
	{
		if (input[current] == '\'' && quote_type == QUOTE_NONE)
			quote_type = QUOTE_SINGLE;
		else if (input[current] == '"' && quote_type == QUOTE_NONE)
			quote_type = QUOTE_DOUBLE;
		else if ((input[current] == '\'' && quote_type == QUOTE_SINGLE)
				|| (input[current] == '"' && quote_type == QUOTE_DOUBLE))
			quote_type = QUOTE_NONE;
		else if (ft_isspace(input[current]) && quote_type == QUOTE_NONE)
			break ;
		current++;
	}
	// Si on a une quote non fermée
	if (quote_type != QUOTE_NONE)
		return (ft_strlen(input) - 1);
	// Extraire la portion complète
	value = ft_substr(input, start, current - start);
	if (!value)
		return (-1);
	// Expansion des variables selon le contexte
	expanded = expand_quoted_word(value, env);
	free(value);
	if (!expanded)
		return (-1);
	new_token = create_token(expanded, TOKEN_WORD, QUOTE_NONE);
	free(expanded);
	if (!new_token)
		return (-1);
	add_token(head, new_token);
	return (current - 1);
}

int	token_handle_redirection(char *input, int i, t_token **head)
{
	char	*value;
	t_token	*new_token;
	int		len;

	len = 1;
	if (input[i + 1] && (ft_strncmp(input + i, ">>", 2) == 0 || ft_strncmp(input
				+ i, "<<", 2) == 0))
		len = 2;
	value = ft_substr(input, i, len);
	if (!value)
		return (-1);
	new_token = create_token(value, determine_token_type(value), QUOTE_NONE);
	free(value);
	if (!new_token)
		return (-1);
	add_token(head, new_token);
	return (i + len - 1);
}
