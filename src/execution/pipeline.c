/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:37:14 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/08 19:09:49 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

void	create_pipe(int pipefd[2])
{
	if (pipe(pipefd) < 0)
		error_exit_message(NULL, NULL, "Error: Pipe creation failed");
}

void	child_process(t_cmd *cmd, t_env **env, int pipefd[2], int is_last)
{
	if (cmd->input_fd != STDIN_FILENO)
	{
		dup2(cmd->input_fd, STDIN_FILENO);
		close(cmd->input_fd);
	}
	if (!is_last)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
	}
	else if (cmd->output_fd != STDOUT_FILENO)
	{
		dup2(cmd->output_fd, STDOUT_FILENO);
		close(cmd->output_fd);
	}
	execute_command(cmd, env);
	exit(EXIT_FAILURE);
}

static void	setup_and_execute_command(t_cmd *current, t_cmd *next, t_env **env,
		int pipefd[2])
{
	pid_t	pid;

	if (next)
		create_pipe(pipefd);
	pid = fork();
	if (pid == 0)
		child_process(current, env, pipefd, next == NULL);
	else if (pid < 0)
		error_exit_message(*env, current, "Error: Fork failed");
	if (next)
	{
		next->input_fd = pipefd[0];
		close(pipefd[1]);
	}
}

void	execute_pipeline(t_cmd *cmd_list, t_env **env)
{
	t_cmd	*current;
	int		pipefd[2];

	current = cmd_list;
	while (current)
	{
		setup_and_execute_command(current, current->next, env, pipefd);
		current = current->next;
	}
	while (wait(NULL) > 0)
		;
}
