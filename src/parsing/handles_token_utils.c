/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handles_token_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:00:39 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/03 18:01:18 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

int	handle_quotes(char *input, int i, t_token **head)
{
	char	quote;
	int		start;

	quote = input[i];
	start = i + 1;
	i++;
	while (input[i] && input[i] != quote)
		i++;
	if (input[i] == quote)
	{
		input[i] = '\0';
		add_token(head, ft_create_token(&input[start], TOKEN_WORD));
		return (i);
	}
	return (start - 1);
}

int	handle_word(char *input, int i, t_token **head)
{
	int		start;
	char	tmp;

	start = i;
	while (input[i] && !ft_isspace(input[i]) && input[i] != '\''
		&& input[i] != '\"')
		i++;
	tmp = input[i];
	input[i] = '\0';
	add_token(head, ft_create_token(&input[start],
			get_token_type(&input[start])));
	input[i] = tmp;
	return (i - 1);
}

int	handle_space(char *input, int i)
{
	while (input[i] && ft_isspace(input[i]))
		i++;
	return (i - 1);
}
