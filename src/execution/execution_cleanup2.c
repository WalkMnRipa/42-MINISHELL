/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_cleanup2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 23:47:49 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/16 23:48:23 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	cleanup_heredoc_chain(t_heredoc *heredoc)
{
	t_heredoc	*next_heredoc;

	while (heredoc)
	{
		next_heredoc = heredoc->next;
		if (heredoc->filename)
		{
			unlink(heredoc->filename);
			free(heredoc->filename);
		}
		if (heredoc->delimiter)
			free(heredoc->delimiter);
		free(heredoc);
		heredoc = next_heredoc;
	}
}

void	cleanup_heredoc_files(t_cmd *cmd)
{
	t_cmd	*current_cmd;

	current_cmd = cmd;
	while (current_cmd)
	{
		if (current_cmd->heredocs)
		{
			cleanup_heredoc_chain(current_cmd->heredocs);
			current_cmd->heredocs = NULL;
		}
		current_cmd = current_cmd->next;
	}
}
