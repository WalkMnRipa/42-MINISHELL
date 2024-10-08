/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:37:14 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/07 17:04:19 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

static void	create_pipe(int pipefd[2])
{
	if (pipe(pipefd) < 0)
		error_exit_message(NULL, NULL, "Error: Pipe creation failed");
}

static void	child_process(t_cmd *cmd, t_env *env, int pipefd[2], int is_last)
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
	exit(EXIT_FAILURE); // In case execute_command fails
}

void	execute_pipeline(t_cmd *cmd_list, t_env *env)
{
	t_cmd	*current;
	int		pipefd[2];
	pid_t	pid;

	current = cmd_list;
	while (current)
	{
		if (current->next)
			create_pipe(pipefd);
		pid = fork();
		if (pid == 0)
		{
			child_process(current, env, pipefd, current->next == NULL);
		}
		else if (pid < 0)
		{
			error_exit_message(env, cmd_list, "Error: Fork failed");
		}
		if (current->next)
		{
			current->next->input_fd = pipefd[0];
			close(pipefd[1]);
		}
		current = current->next;
	}
	// Wait for all child processes
	while (wait(NULL) > 0)
		;
}
