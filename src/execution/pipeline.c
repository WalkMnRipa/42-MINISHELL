/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:37:14 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/16 16:34:12 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*#include "../../includes/execution.h"

void	execute_pipeline(t_cmd *cmd, t_env **env)
{
	int		current_pipe;
	t_cmd	*current;
	pid_t	*pids;
	int		cmd_count;
	int		i;
	int		status;

	int pipe_fds[2][2]; // Two sets of pipe file descriptors
	current_pipe = 0;
	current = cmd;
	pids = NULL;
	cmd_count = 0;
	// Count commands
	for (t_cmd *tmp = cmd; tmp; tmp = tmp->next)
		cmd_count++;
	// Allocate pid array
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
		return ;
	i = 0;
	while (current)
	{
		// Create pipe for all but the last command
		if (current->next && pipe(pipe_fds[current_pipe]) == -1)
		{
			perror("pipe");
			free(pids);
			return ;
		}
		pids[i] = fork();
		if (pids[i] == -1)
		{
			perror("fork");
			free(pids);
			return ;
		}
		if (pids[i] == 0)
		{
			// Child process
			if (i > 0) // Not first command
			{
				close(pipe_fds[!current_pipe][1]);
				dup2(pipe_fds[!current_pipe][0], STDIN_FILENO);
				close(pipe_fds[!current_pipe][0]);
			}
			if (current->next) // Not last command
			{
				close(pipe_fds[current_pipe][0]);
				dup2(pipe_fds[current_pipe][1], STDOUT_FILENO);
				close(pipe_fds[current_pipe][1]);
			}
			// Execute the command
			if (is_builtin(current->args[0]))
				execute_builtin(current, env);
			else
				execute_external_command(current, env);
			exit(current->exit_status);
		}
		// Parent process
		if (i > 0)
		{
			close(pipe_fds[!current_pipe][0]);
			close(pipe_fds[!current_pipe][1]);
		}
		current_pipe = !current_pipe;
		current = current->next;
		i++;
	}
	// Close remaining pipe ends
	if (cmd_count > 1)
	{
		close(pipe_fds[!current_pipe][0]);
		close(pipe_fds[!current_pipe][1]);
	}
	// Wait for all processes
	for (i = 0; i < cmd_count; i++)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1) // Last command
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
	}
	free(pids);
}*/
