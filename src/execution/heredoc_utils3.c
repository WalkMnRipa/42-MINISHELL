/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 10:48:00 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/02 11:21:23 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_heredoc	*get_last_heredoc(t_cmd *cmd)
{
	t_heredoc	*current;

	current = cmd->heredocs;
	while (current && current->next)
		current = current->next;
	return (current);
}

static int	set_input_from_last_heredoc(t_cmd *cmd)
{
	t_heredoc	*last;

	if (!cmd->input_file)
	{
		last = get_last_heredoc(cmd);
		if (last)
		{
			cmd->input_file = ft_strdup(last->filename);
			if (!cmd->input_file)
				return (1);
		}
	}
	return (0);
}

int	handle_multiple_heredocs(t_cmd *cmd, t_env *env)
{
	t_heredoc	*current;
	int			status;

	setup_heredoc_signals();
	current = cmd->heredocs;
	while (current)
	{
		status = write_heredoc_content(current, env);
		if (status)
		{
			cmd->exit_status = 1;
			setup_signals();
			return (1);
		}
		current = current->next;
	}
	if (cmd->heredocs && set_input_from_last_heredoc(cmd))
	{
		setup_signals();
		return (1);
	}
	setup_signals();
	return (0);
}
