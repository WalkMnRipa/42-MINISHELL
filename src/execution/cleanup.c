/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 16:19:03 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/17 17:14:17 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_string_array(char **array, int count)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	if (count == -1)
	{
		while (array[i])
		{
			cleanup_ptr(array[i]);
			i++;
		}
	}
	else
	{
		while (i < count)
		{
			if (array[i])
				cleanup_ptr(array[i]);
			i++;
		}
	}
	cleanup_ptr(array);
}

static void	cleanup_heredoc_chain(t_heredoc *heredoc)
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
		if (heredoc->delimiter)
			cleanup_ptr(heredoc->delimiter);
		cleanup_ptr(heredoc);
		heredoc = next;
	}
}

static void	cleanup_cmd(t_cmd *cmd)
{
	t_cmd	*next;

	while (cmd)
	{
		next = cmd->next;
		if (cmd->args)
			free_string_array(cmd->args, -1);
		cleanup_ptr(cmd->input_file);
		cleanup_ptr(cmd->output_file);
		if (cmd->input_fd != STDIN_FILENO)
			close(cmd->input_fd);
		if (cmd->output_fd != STDOUT_FILENO)
			close(cmd->output_fd);
		if (cmd->heredocs)
			cleanup_heredoc_chain(cmd->heredocs);
		cleanup_ptr(cmd);
		cmd = next;
	}
}

static void	cleanup_env(t_env *env)
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

void	cleanup_all(t_env *env, t_cmd *cmd, int exit_code)
{
	cleanup_cmd(cmd);
	cleanup_env(env);
	if (exit_code != -1)
	{
		rl_clear_history();
		clear_history();
		exit(exit_code);
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

void	cleanup_heredoc_files(t_cmd *cmd)
{
	t_cmd		*current_cmd;
	t_heredoc	*next_heredoc;
	t_heredoc	*current_heredoc;

	current_cmd = cmd;
	while (current_cmd)
	{
		if (current_cmd->heredocs)
		{
			current_heredoc = current_cmd->heredocs;
			while (current_heredoc)
			{
				next_heredoc = current_heredoc->next;
				if (current_heredoc->filename)
				{
					unlink(current_heredoc->filename);
					free(current_heredoc->filename);
				}
				if (current_heredoc->delimiter)
					free(current_heredoc->delimiter);
				free(current_heredoc);
				current_heredoc = next_heredoc;
			}
			current_cmd->heredocs = NULL;
		}
		current_cmd = current_cmd->next;
	}
}

void	cleanup_readline(void)
{
	rl_clear_history();
	clear_history();
	rl_free_line_state();
	rl_cleanup_after_signal();
}
