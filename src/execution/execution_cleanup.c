/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_cleanup.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 20:11:10 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/17 13:42:53 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

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
			free(cmd->input_file);
		if (cmd->output_file)
			free(cmd->output_file);
		if (cmd->input_fd != STDIN_FILENO)
			close(cmd->input_fd);
		if (cmd->output_fd != STDOUT_FILENO)
			close(cmd->output_fd);
		free(cmd);
	}
}

void	cleanup(t_env *env, t_cmd *cmd)
{
	t_cmd	*next;

	free_env(env);
	while (cmd)
	{
		next = cmd->next;
		free_cmd(cmd);
		cmd = next;
	}
}

void	error_exit_message(t_env *env, t_cmd *cmd, char *message)
{
	if (message)
		ft_putendl_fd(message, 2);
	cleanup(env, cmd);
	exit(EXIT_FAILURE);
}
