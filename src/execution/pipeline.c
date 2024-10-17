/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:37:14 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/17 14:40:58 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

static void	setup_child_process(int input_fd, int output_fd)
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
}

static void	execute_piped_command(t_cmd *cmd, t_env **env, int in_fd,
		int out_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		error_exit_message(*env, cmd, "Fork failed");
	else if (pid == 0)
	{
		setup_child_process(in_fd, out_fd);
		execute_command(cmd, env);
		exit(cmd->exit_status);
	}
}

static void	close_pipe_ends(int prev_pipe, int *pipefd, t_cmd *current)
{
	if (prev_pipe != STDIN_FILENO)
		close(prev_pipe);
	if (current->next)
	{
		close(pipefd[1]);
		prev_pipe = pipefd[0];
	}
}

static int	setup_pipeline_and_get_output(t_cmd *current, int *pipefd,
		t_env **env)
{
	if (current->next)
	{
		if (pipe(pipefd) == -1)
			error_exit_message(*env, current, "Pipe creation failed");
		return (pipefd[1]);
	}
	return (STDOUT_FILENO);
}

void	execute_pipeline(t_cmd *cmd_list, t_env **env)
{
	int pipefd[2];
	int prev_pipe;
	t_cmd *current;
	int output_fd;

	prev_pipe = STDIN_FILENO;
	current = cmd_list;
	while (current)
	{
		output_fd = setup_pipeline_and_get_output(current, pipefd, env);
		execute_piped_command(current, env, prev_pipe, output_fd);
		close_pipe_ends(prev_pipe, pipefd, current);
		if (current->next)
			prev_pipe = pipefd[0];
		current = current->next;
	}
	while (wait(NULL) > 0)
		;
}
