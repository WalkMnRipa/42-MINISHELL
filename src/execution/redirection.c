/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 17:39:24 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/15 17:42:37 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

static int	open_input_file(t_cmd *cmd)
{
	int	fd;

	fd = open(cmd->input_file, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->input_file, 2);
		ft_putendl_fd(": No such file or directory", 2);
		return (-1);
	}
	return (fd);
}

static int	handle_input_redirection(t_cmd *cmd)
{
	int	fd;

	if (!cmd->input_file)
		return (1);
	fd = open_input_file(cmd);
	if (fd == -1)
		return (0);
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (1);
}

static int	open_output_file(t_cmd *cmd)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (cmd->append_output)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(cmd->output_file, flags, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->output_file, 2);
		ft_putendl_fd(": Permission denied", 2);
		return (-1);
	}
	return (fd);
}

static int	handle_output_redirection(t_cmd *cmd)
{
	int	fd;

	if (!cmd->output_file)
		return (1);
	fd = open_output_file(cmd);
	if (fd == -1)
		return (0);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (1);
}

int	setup_redirections(t_cmd *cmd)
{
	if (!handle_input_redirection(cmd))
		return (0);
	if (!handle_output_redirection(cmd))
		return (0);
	return (1);
}
