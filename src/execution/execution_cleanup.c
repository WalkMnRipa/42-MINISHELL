/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_cleanup.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 20:11:10 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/16 15:54:54 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_cmd(t_cmd *cmd)
{
	char	**tmp;

	if (!cmd)
		return ;
	if (cmd->args)
	{
		tmp = cmd->args;
		while (*tmp)
		{
			free(*tmp);
			tmp++;
		}
		free(cmd->args);
	}
	if (cmd->input_file)
		free(cmd->input_file);
	if (cmd->output_file)
		free(cmd->output_file);
	if (cmd->heredocs)
		free_heredocs(cmd->heredocs);
	if (cmd->input_fd != STDIN_FILENO)
		close(cmd->input_fd);
	if (cmd->output_fd != STDOUT_FILENO)
		close(cmd->output_fd);
	free(cmd);
}

void	cleanup(t_env *env, t_cmd *cmd)
{
	t_cmd	*next;
	t_env	*next_env;

	while (cmd)
	{
		next = cmd->next;
		free_cmd(cmd);
		cmd = next;
	}
	while (env)
	{
		next_env = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = next_env;
	}
}

void	error_exit_message(t_env *env, t_cmd *cmd, char *message)
{
	if (message)
		ft_putendl_fd(message, 2);
	cleanup(env, cmd);
	exit(EXIT_FAILURE);
}

void	cleanup_heredoc_files(t_cmd *cmd)
{
	t_cmd		*current_cmd;
	t_heredoc	*current_heredoc;

	current_cmd = cmd;
	while (current_cmd)
	{
		if (current_cmd->heredocs)
		{
			current_heredoc = current_cmd->heredocs;
			while (current_heredoc)
			{
				if (current_heredoc->filename)
				{
					unlink(current_heredoc->filename);
					free(current_heredoc->filename);
					current_heredoc->filename = NULL;
				}
				current_heredoc = current_heredoc->next;
			}
		}
		current_cmd = current_cmd->next;
	}
}
