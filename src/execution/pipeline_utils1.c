/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 17:14:39 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/17 17:34:31 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_pipe_process(t_cmd *cmd, t_pipe_data *data)
{
	if (!handle_pipe_creation(cmd, data->pipe_fds, data->info->current_pipe))
	{
		cleanup_ptr(data->pids);
		(*data->env)->last_exit_status = 1;
		return (0);
	}
	data->pids[data->info->index] = create_process(cmd, data->env,
			data->pipe_fds, data->info);
	if (data->pids[data->info->index] == -1)
	{
		cleanup_ptr(data->pids);
		(*data->env)->last_exit_status = 1;
		return (0);
	}
	handle_parent_pipes(data->pipe_fds, data->info->index,
		data->info->current_pipe);
	return (1);
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

void	handle_parent_pipes(int pipe_fds[2][2], int i, int current_pipe)
{
	if (i > 0)
	{
		close(pipe_fds[!current_pipe][0]);
		close(pipe_fds[!current_pipe][1]);
	}
}
