/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 20:41:46 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/17 20:46:26 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	cleanup_env(t_env *env)
{
	t_env	*next;

	while (env)
	{
		next = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = next;
	}
}

void	cleanup_ptr(void *ptr)
{
	if (ptr)
		free(ptr);
}

void	error_exit_message(t_env *env, t_cmd *cmd, char *message)
{
	if (message)
		ft_putendl_fd(message, 2);
	cleanup_all(env, cmd, EXIT_FAILURE);
}

static void	free_heredoc(t_heredoc *heredoc)
{
	if (heredoc->filename)
	{
		unlink(heredoc->filename);
		free(heredoc->filename);
	}
	if (heredoc->delimiter)
		free(heredoc->delimiter);
	free(heredoc);
}

void	cleanup_heredoc_files(t_cmd *cmd)
{
	t_cmd		*current_cmd;
	t_heredoc	*current_heredoc;
	t_heredoc	*next_heredoc;

	current_cmd = cmd;
	while (current_cmd)
	{
		if (current_cmd->heredocs)
		{
			current_heredoc = current_cmd->heredocs;
			while (current_heredoc)
			{
				next_heredoc = current_heredoc->next;
				free_heredoc(current_heredoc);
				current_heredoc = next_heredoc;
			}
			current_cmd->heredocs = NULL;
		}
		current_cmd = current_cmd->next;
	}
}
