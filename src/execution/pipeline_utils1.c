/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 17:14:39 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/25 16:47:51 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

static int	handle_pipe_process(t_cmd *cmd, t_pipe_data *data)
{
	if (!handle_pipe_creation(cmd, data->pipe_fds, data->info->current_pipe))
	{
		cleanup_pipeline(data->pids, data->pipe_fds, data->info->index);
		(*data->env)->last_exit_status = 1;
		return (0);
	}
	data->pids[data->info->index] = create_process(cmd, data->env,
			data->pipe_fds, data->info);
	if (data->pids[data->info->index] == -1)
	{
		cleanup_pipeline(data->pids, data->pipe_fds, data->info->index);
		(*data->env)->last_exit_status = 1;
		return (0);
	}
	handle_parent_pipes(data->pipe_fds, data->info->index,
		data->info->current_pipe);
	return (1);
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
			return ;
		info->current_pipe = !info->current_pipe;
		cmd = cmd->next;
		info->index++;
	}
}

static void	execute_child_process(t_cmd *cmd, t_env **env)
{
	reset_signals();
	if (is_builtin(cmd->args[0]))
		execute_builtin(cmd, env);
	else
		execute_external_command(cmd, env);
	exit(cmd->exit_status);
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
			exit(1);
		execute_child_process(cmd, env);
	}
	return (pid);
}
