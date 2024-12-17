/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:48:24 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/17 17:34:05 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

static void	execute_child_process(t_cmd *cmd, t_env **env)
{
	reset_signals();
	if (is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd, env);
		cleanup_all(*env, cmd, cmd->exit_status);
	}
	else
		execute_external_command(cmd, env);
}

pid_t	create_process(t_cmd *cmd, t_env **env, int pipe_fds[2][2],
		t_pipe_info *info)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
	{
		setup_child_pipes(pipe_fds, info->index, info->current_pipe,
			cmd->next != NULL);
		if (!setup_redirections(cmd))
		{
			if (cmd->next)
				cleanup_all(*env, cmd, 0);
			else
				cleanup_all(*env, cmd, 1);
		}
		execute_child_process(cmd, env);
	}
	return (pid);
}

void	run_pipeline_loop(t_cmd *cmd, pid_t *pids, t_pipe_info *info,
		t_env **env)
{
	t_pipe_data	data;

	data.pids = pids;
	data.info = info;
	data.env = env;
	while (cmd && info->index < info->cmd_count)
	{
		if (!handle_pipe_process(cmd, &data))
		{
			cleanup_ptr(pids);
			return ;
		}
		info->current_pipe = !info->current_pipe;
		cmd = cmd->next;
		info->index++;
	}
}
