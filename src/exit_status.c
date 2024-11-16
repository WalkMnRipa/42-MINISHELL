/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:29:07 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/16 17:20:00 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

void	update_exit_status(t_cmd *cmd, int status)
{
	if (WIFEXITED(status))
		cmd->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		cmd->exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
	}
}

void	execute_non_builtin(t_cmd *cmd, t_env **env)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		execute_external_command(cmd, env);
		exit(cmd->exit_status);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		update_exit_status(cmd, status);
	}
	else
	{
		perror("fork");
		cmd->exit_status = 1;
	}
}
