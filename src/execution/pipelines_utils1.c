/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipelines_utils1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 17:14:39 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/18 23:40:32 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

static int	handle_pipe_process(t_cmd *cmd, pid_t *pids, int pipe_fds[2][2],
		t_pipe_info *info, t_env **env)
{
	if (!handle_pipe_creation(cmd, pipe_fds, info->current_pipe))
		return (cleanup_pipeline(pids, pipe_fds, info->index),
			(*env)->last_exit_status = 1, 0);
	pids[info->index] = create_process(cmd, env, pipe_fds, info);
	if (pids[info->index] == -1)
		return (cleanup_pipeline(pids, pipe_fds, info->index),
			(*env)->last_exit_status = 1, 0);
	handle_parent_pipes(pipe_fds, info->index, info->current_pipe);
	return (1);
}

void	run_pipeline_loop(t_cmd *cmd, pid_t *pids, t_pipe_info *info,
		t_env **env)
{
	int	pipe_fds[2][2];

	while (cmd && info->index < info->cmd_count)
	{
		if (!handle_pipe_process(cmd, pids, pipe_fds, info, env))
			return ;
		info->current_pipe = !info->current_pipe;
		cmd = cmd->next;
		info->index++;
	}
}
