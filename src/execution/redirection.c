/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 17:39:24 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/15 20:12:51 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

int	setup_redirections(t_cmd *cmd)
{
	int	flags;
	int	fd;

	if (!cmd)
		return (0);
	if (cmd->input_file)
	{
		fd = open(cmd->input_file, O_RDONLY);
		if (fd == -1)
		{
			perror("minishell: open input file");
			return (0);
		}
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			perror("minishell: dup2 input");
			close(fd);
			return (0);
		}
		close(fd);
	}
	if (cmd->output_file)
	{
		if (cmd->append_output)
			flags = O_WRONLY | O_CREAT | O_APPEND;
		else
			flags = O_WRONLY | O_CREAT | O_TRUNC;
		fd = open(cmd->output_file, flags, 0644);
		if (fd == -1)
		{
			perror("minishell: open output file");
			return (0);
		}
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2 output");
			close(fd);
			return (0);
		}
		close(fd);
	}
	return (1);
}
