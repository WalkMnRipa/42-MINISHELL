/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 01:05:48 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/01 21:34:19 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_string_array(char **array, int count)
{
	int	i;

	if (!array)
		return ;
	if (count == -1)
	{
		i = 0;
		while (array[i])
		{
			free(array[i]);
			i++;
		}
	}
	else
	{
		i = 0;
		while (i < count)
		{
			if (array[i])
				free(array[i]);
			i++;
		}
	}
	free(array);
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
			free_string_array(current->args, -1);
		if (current->input_file)
			free(current->input_file);
		if (current->output_file)
			free(current->output_file);
		if (current->input_fd != STDIN_FILENO)
			close(current->input_fd);
		if (current->output_fd != STDOUT_FILENO)
			close(current->output_fd);
		free(current);
		current = next;
	}
}
