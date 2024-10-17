/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:37:14 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/17 17:19:51 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

static void	child_process(t_cmd *cmd, int input_fd, int output_fd, t_env **env)
{
	if (input_fd != STDIN_FILENO)
	{
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
	if (output_fd != STDOUT_FILENO)
	{
		dup2(output_fd, STDOUT_FILENO);
		close(output_fd);
	}
	execute_single_command(cmd, env);
	exit(cmd->exit_status);
}

static void	setup_pipes(int *input_fd, int *fd, t_cmd *cmd)
{
	if (cmd->next)
	{
		pipe(fd);
		*input_fd = fd[0];
	}
	else
		*input_fd = STDIN_FILENO;
}

void	execute_pipeline(t_cmd *cmd, t_env **env)
{
	int	fd[2];
	int	input_fd;

	input_fd = dup(STDIN_FILENO);
	while (cmd)
	{
		setup_pipes(&input_fd, fd, cmd);
		if (fork() == 0)
		{
			close(fd[0]);
			dup2(fd[1], STDOUT_FILENO);
			close(fd[1]);
			child_process(cmd, input_fd, fd[1], env);
		}
		close(fd[1]);
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
		cmd = cmd->next;
	}
	dup2(input_fd, STDIN_FILENO);
	close(input_fd);
	while (wait(NULL) > 0)
		;
}

void	execute_single_command(t_cmd *cmd, t_env **env)
{
	if (is_builtin(cmd->args[0]))
		execute_builtin(cmd, env);
	else
		execute_external_command(cmd, env);
}
