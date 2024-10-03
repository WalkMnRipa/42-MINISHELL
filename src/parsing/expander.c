/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 16:09:22 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/03 18:27:17 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

void	expand_tokens(t_token *head, char **env)
{
	t_token	*current;
	char	*expanded;

	current = head;
	while (current)
	{
		if (current->type == TOKEN_WORD && (current->quote_type == QUOTE_NONE
				|| current->quote_type == QUOTE_DOUBLE))
		{
			expanded = expand_variables(current->value, env);
			free(current->value);
			current->value = expanded;
		}
		current = current->next;
	}
}
