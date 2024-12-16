/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:28:40 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/16 18:39:32 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	cleanup_pipes(int pipe_fds[2][2], pid_t *pids)
{
	int	i;

	i = 0;
	while (i < 2)
	{
		if (pipe_fds[i][0] != -1)
			close(pipe_fds[i][0]);
		if (pipe_fds[i][1] != -1)
			close(pipe_fds[i][1]);
		i++;
	}
	free(pids);
}

static int	init_pipeline(t_cmd *cmd, pid_t **pids, t_pipe_info *info)
{
	t_cmd	*current;

	info->cmd_count = 0;
	current = cmd;
	while (current)
	{
		info->cmd_count++;
		current = current->next;
	}
	*pids = malloc(sizeof(pid_t) * (info->cmd_count));
	if (!(*pids))
		return (0);
	info->index = 0;
	info->current_pipe = 0;
	return (1);
}

static int	setup_pipeline(t_cmd *cmd, pid_t **pids, t_pipe_info *info,
		t_env **env)
{
	if (!init_pipeline(cmd, pids, info))
	{
		ft_putendl_fd("minishell: memory allocation failed", 2);
		cleanup_heredoc_files(cmd);
		return (0);
	}
	if (handle_multiple_heredocs(cmd, *env) != 0)
	{
		free(*pids);
		return (0);
	}
	return (1);
}

static void	wait_for_children(pid_t *pids, int cmd_count, t_env **env,
		t_cmd *first_cmd)
{
	int	i;
	int	status;
	int	pipeline_error;

	i = 0;
	pipeline_error = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1)
		{
			handle_last_process_status(status, env);
			first_cmd->exit_status = (*env)->last_exit_status;
		}
		else if ((WIFEXITED(status) && WEXITSTATUS(status) != 0)
			|| WIFSIGNALED(status))
			pipeline_error = 1;
		i++;
	}
	if (pipeline_error)
		cleanup_heredoc_files(first_cmd);
}

void	execute_pipeline(t_cmd *cmd, t_env **env)
{
	t_pipe_info	info;
	pid_t		*pids;
	t_cmd		*first_cmd;
	int			pipe_fds[2][2];

	first_cmd = cmd;
	ft_memset(pipe_fds, -1, sizeof(pipe_fds));
	if (!setup_pipeline(cmd, &pids, &info, env))
		return ;
	setup_parent_signals();
	run_pipeline_loop(cmd, pids, &info, env);
	if (info.index == info.cmd_count)
		wait_for_children(pids, info.cmd_count, env, first_cmd);
	cleanup_pipes(pipe_fds, pids);
	setup_signals();
}
