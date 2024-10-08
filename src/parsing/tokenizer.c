/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 18:30:32 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/08 16:58:46 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

static int	process_token(char *input, int i, t_token **head)
{
	if (input[i] == '\'')
		return (handle_single_quotes(input, i, head));
	else if (input[i] == '"')
		return (handle_double_quotes(input, i, head));
	else if (ft_isspace(input[i]))
		return (handle_space(input, i));
	else
		return (handle_word(input, i, head));
}

static t_token	*tokenize_input(char *input)
{
	t_token	*head;
	int		i;
	int		new_i;

	head = NULL;
	i = 0;
	while (input[i])
	{
		new_i = process_token(input, i, &head);
		if (new_i < 0)
			return (free_tokens(head), NULL);
		i = new_i + 1; // +1 pour passer au caractere suivant apres le token
	}
	return (head);
}

t_token	*ft_tokenizer(char *input)
{
	t_token			*tokens;
	t_parse_error	error;

	if (!input)
		return (NULL);
	tokens = tokenize_input(input);
	if (!tokens)
		return (NULL); // Erreur deja geree dans tokenize_input
	error = check_token_syntax(tokens);
	if (error != PARSE_ERROR_NONE)
	{
		ft_handle_parse_error(error);
		return (free_tokens(tokens), NULL);
	}
	return (tokens);
}
