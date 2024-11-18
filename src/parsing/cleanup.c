/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 19:54:50 by jcohen            #+#    #+#             */
/*   Updated: 2024/11/18 15:31:38 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

void	free_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current)
	{
		next = current->next;
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
}

void	free_cmd_list(t_cmd *head)
{
	t_cmd	*current;
	t_cmd	*next;

	current = head;
	while (current)
	{
		next = current->next;
		if (current->args)
			free(current->args);
		free(current->input_file);
		free(current->output_file);
		free(current);
		current = next;
	}
}

void	free_string_array(char **array, int count)
{
	if (array)
	{
		while (count--)
			free(array[count]);
		free(array);
	}
}
