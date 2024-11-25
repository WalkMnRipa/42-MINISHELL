/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:48:24 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/25 04:34:08 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

void	setup_child_pipes(int pipe_fds[2][2], int i, int current_pipe,
		int has_next)
{
	if (i > 0)
	{
		close(pipe_fds[!current_pipe][1]);
		dup2(pipe_fds[!current_pipe][0], STDIN_FILENO);
		close(pipe_fds[!current_pipe][0]);
	}
	if (has_next)
	{
		close(pipe_fds[current_pipe][0]);
		dup2(pipe_fds[current_pipe][1], STDOUT_FILENO);
		close(pipe_fds[current_pipe][1]);
	}
}

int	handle_pipe_creation(t_cmd *cmd, int pipe_fds[2][2], int current_pipe)
{
	if (cmd->next && pipe(pipe_fds[current_pipe]) == -1)
	{
		perror("pipe");
		return (0);
	}
	return (1);
}

void	handle_parent_pipes(int pipe_fds[2][2], int i, int current_pipe)
{
	if (i > 0)
	{
		close(pipe_fds[!current_pipe][0]);
		close(pipe_fds[!current_pipe][1]);
	}
}

void	cleanup_pipeline(pid_t *pids, int pipe_fds[2][2], int i)
{
	if (i > 0)
	{
		close(pipe_fds[0][0]);
		close(pipe_fds[0][1]);
		close(pipe_fds[1][0]);
		close(pipe_fds[1][1]);
	}
	free(pids);
}

void	handle_last_process_status(int status, t_env **env)
{
	if (WIFEXITED(status))
		(*env)->last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		(*env)->last_exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
	}
}
