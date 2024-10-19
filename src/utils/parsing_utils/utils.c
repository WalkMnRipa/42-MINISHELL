/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 18:55:18 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/19 19:49:43 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parsing.h"

int	token_handle_space(char *input, int i)
{
	while (input[i] && ft_isspace(input[i]))
		i++;
	return (i - 1);
}

int	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

int	is_valid_var_start(char c)
{
	return (is_valid_var_char(c) || c == '?' || c == '$');
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (s1);
	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}
