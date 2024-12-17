/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 21:36:33 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/17 16:28:28 by ggaribot         ###   ########.fr       */
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
	cleanup_ptr(num_str);
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
		cleanup_ptr(new_heredoc->delimiter);
		cleanup_ptr(new_heredoc->filename);
		cleanup_ptr(new_heredoc);
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
			cleanup_ptr(heredoc->filename);
		}
		cleanup_ptr(heredoc->delimiter);
		cleanup_ptr(heredoc);
		heredoc = next;
	}
}

int	setup_last_heredoc(t_cmd *cmd)
{
	t_heredoc	*current;
	char		*last_filename;

	if (!cmd->heredocs || cmd->input_file)
		return (0);
	current = cmd->heredocs;
	while (current->next)
		current = current->next;
	last_filename = ft_strdup(current->filename);
	if (!last_filename)
		return (1);
	cmd->input_file = last_filename;
	current = cmd->heredocs;
	while (current)
	{
		if (current->filename && ft_strcmp(current->filename,
				last_filename) != 0)
		{
			unlink(current->filename);
			cleanup_ptr(current->filename);
			current->filename = NULL;
		}
		current = current->next;
	}
	return (0);
}
