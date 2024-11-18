/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 17:39:24 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/18 15:33:02 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

static int	setup_input_redirection(t_cmd *cmd)
{
	int	fd;

	if (cmd->input_fd != STDIN_FILENO)
	{
		dup2(cmd->input_fd, STDIN_FILENO);
		close(cmd->input_fd);
		cmd->input_fd = STDIN_FILENO;
		return (1);
	}
	if (cmd->input_file)
	{
		fd = open(cmd->input_file, O_RDONLY);
		if (fd == -1)
		{
			perror("minishell: input redirection");
			return (0);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	return (1);
}

static int	setup_output_redirection(t_cmd *cmd)
{
	int	fd;
	int	flags;

	if (cmd->output_file)
	{
		if (cmd->append_output)
			flags = O_WRONLY | O_CREAT | O_APPEND;
		else
			flags = O_WRONLY | O_CREAT | O_TRUNC;
		fd = open(cmd->output_file, flags, 0644);
		if (fd == -1)
		{
			perror("minishell: output redirection");
			return (0);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	return (1);
}

int	setup_redirections(t_cmd *cmd)
{
	if (!setup_input_redirection(cmd))
		return (0);
	if (!setup_output_redirection(cmd))
		return (0);
	return (1);
}
