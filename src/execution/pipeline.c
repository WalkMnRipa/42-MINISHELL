/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:28:40 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/25 03:13:02 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"
/*
static void	wait_for_processes(pid_t *pids, int cmd_count, t_env **env)
{
	int	i;
	int	status;

	i = -1;
	while (++i < cmd_count)
	{
		status = 0;
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1)
		{
			if (WIFSIGNALED(status))
			{
				(*env)->last_exit_status = 128 + WTERMSIG(status);
				if (WTERMSIG(status) == SIGQUIT)
					ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
			}
			else if (WIFEXITED(status))
				(*env)->last_exit_status = WEXITSTATUS(status);
		}
	}
}
*/
static void	execute_child_process(t_cmd *cmd, t_env **env)
{
	reset_signals();
	if (is_builtin(cmd->args[0]))
		execute_builtin(cmd, env);
	else
		execute_external_command(cmd, env);
	exit(cmd->exit_status);
}

pid_t create_process(t_cmd *cmd, t_env **env, int pipe_fds[2][2], t_pipe_info *info)
{
    pid_t pid;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return (-1);
    }
    if (pid == 0)
    {
        // First set up pipes
        setup_child_pipes(pipe_fds, info->index, info->current_pipe,
            cmd->next != NULL);
        // Then handle redirections
        if (!setup_redirections(cmd))
            exit(1);
        execute_child_process(cmd, env);
    }
    return (pid);
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

void execute_pipeline(t_cmd *cmd, t_env **env)
{
    t_pipe_info  info;
    pid_t        *pids;
    t_cmd        *first_cmd;
    int          pipe_fds[2][2] = {{-1, -1}, {-1, -1}};
    int          pipe_status;

    first_cmd = cmd;
    if (!init_pipeline(cmd, &pids, &info))
    {
        ft_putendl_fd("minishell: memory allocation failed", 2);
        return;
    }

    setup_parent_signals();
    while (cmd && info.index < info.cmd_count)
    {
        if (cmd->next)
        {
            pipe_status = handle_pipe_creation(cmd, pipe_fds, info.current_pipe);
            if (!pipe_status)
            {
                cleanup_pipeline(pids, pipe_fds, info.index);
                (*env)->last_exit_status = 1;
                return;
            }
        }

        pids[info.index] = create_process(cmd, env, pipe_fds, &info);
        if (pids[info.index] == -1)
        {
            cleanup_pipeline(pids, pipe_fds, info.index);
            (*env)->last_exit_status = 1;
            return;
        }

        if (info.index > 0)
        {
            close(pipe_fds[!info.current_pipe][0]);
            close(pipe_fds[!info.current_pipe][1]);
            pipe_fds[!info.current_pipe][0] = -1;
            pipe_fds[!info.current_pipe][1] = -1;
        }

        info.current_pipe = !info.current_pipe;
        cmd = cmd->next;
        info.index++;
    }

    if (info.index == info.cmd_count)
    {
        int status;
        for (int i = 0; i < info.cmd_count; i++)
        {
            waitpid(pids[i], &status, 0);
            if (i == info.cmd_count - 1)
            {
                handle_last_process_status(status, env);
                first_cmd->exit_status = (*env)->last_exit_status;
            }
        }
    }

    for (int i = 0; i < 2; i++)
    {
        if (pipe_fds[i][0] != -1) close(pipe_fds[i][0]);
        if (pipe_fds[i][1] != -1) close(pipe_fds[i][1]);
    }

    setup_signals();
    free(pids);
}