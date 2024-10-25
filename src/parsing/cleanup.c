/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 19:54:50 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/25 16:47:16 by jcohen           ###   ########.fr       */
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

void	free_cmd(t_cmd *cmd)
{
	char	**args;

	if (cmd)
	{
		args = cmd->args;
		while (*args)
		{
			free(*args);
			args++;
		}
		free(cmd->args);
		if (cmd->input_file)
		{
			if (ft_strcmp(cmd->input_file, HEREDOC_TMP) == 0)
				unlink(HEREDOC_TMP);
			free(cmd->input_file);
		}
		if (cmd->output_file)
			free(cmd->output_file);
		if (cmd->input_fd != STDIN_FILENO)
			close(cmd->input_fd);
		if (cmd->output_fd != STDOUT_FILENO)
			close(cmd->output_fd);
		free(cmd);
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
