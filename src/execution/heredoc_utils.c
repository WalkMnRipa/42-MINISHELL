/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 21:36:33 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/02 15:16:09 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*generate_heredoc_filename(void)
{
	static int	count = 0;
	char		*num_str;
	char		*filename;

	num_str = ft_itoa(count++);
	if (!num_str)
		return (NULL);
	filename = ft_strjoin(".heredoc_tmp_", num_str);
	free(num_str);
	return (filename);
}

t_heredoc	*create_heredoc(char *delimiter)
{
	t_heredoc	*new_heredoc;

	new_heredoc = malloc(sizeof(t_heredoc));
	if (!new_heredoc)
		return (NULL);
	new_heredoc->delimiter = ft_strdup(delimiter);
	new_heredoc->filename = generate_heredoc_filename();
	new_heredoc->expand_vars = (ft_strchr(delimiter, '\'') == NULL);
	new_heredoc->next = NULL;
	if (!new_heredoc->delimiter || !new_heredoc->filename)
	{
		free(new_heredoc->delimiter);
		free(new_heredoc->filename);
		free(new_heredoc);
		return (NULL);
	}
	return (new_heredoc);
}

void	free_heredocs(t_heredoc *heredoc)
{
	t_heredoc	*next;

	while (heredoc)
	{
		next = heredoc->next;
		if (heredoc->filename)
		{
			unlink(heredoc->filename);
			free(heredoc->filename);
		}
		if (heredoc->delimiter)
			free(heredoc->delimiter);
		free(heredoc);
		heredoc = next;
	}
}

int	setup_last_heredoc(t_cmd *cmd)
{
	t_heredoc	*current;

	if (!cmd->heredocs || cmd->input_file)
		return (0);
	current = cmd->heredocs;
	while (current->next)
		current = current->next;
	cmd->input_file = ft_strdup(current->filename);
	if (!cmd->input_file)
		return (1);
	current = cmd->heredocs;
	while (current)
	{
		if (current->filename && (!cmd->input_file
				|| ft_strcmp(current->filename, cmd->input_file) != 0))
			unlink(current->filename);
		current = current->next;
	}
	return (0);
}
