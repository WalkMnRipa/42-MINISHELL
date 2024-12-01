/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 16:36:46 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/01 13:32:16 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	cleanup_fds(int prev_in_fd, int prev_out_fd)
{
	if (prev_in_fd != -1)
		close(prev_in_fd);
	if (prev_out_fd != -1)
		close(prev_out_fd);
}

int	check_input_permissions(t_cmd *cmd, int *prev_fds)
{
	if (access(cmd->input_file, F_OK) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->input_file, 2);
		ft_putendl_fd(": No such file or directory", 2);
		cleanup_fds(prev_fds[0], prev_fds[1]);
		return (0);
	}
	if (access(cmd->input_file, R_OK) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->input_file, 2);
		ft_putendl_fd(": Permission denied", 2);
		cleanup_fds(prev_fds[0], prev_fds[1]);
		return (0);
	}
	return (1);
}

static void	restore_fds(int *prev_fds)
{
	dup2(prev_fds[0], STDIN_FILENO);
	dup2(prev_fds[1], STDOUT_FILENO);
	cleanup_fds(prev_fds[0], prev_fds[1]);
}

int	check_output_permissions(t_cmd *cmd, int *prev_fds)
{
	char	*dir_path;
	char	*last_slash;

	dir_path = ft_strdup(cmd->output_file);
	if (!dir_path)
		return (0);
	last_slash = ft_strrchr(dir_path, '/');
	if (last_slash)
	{
		*last_slash = '\0';
		if (access(dir_path, W_OK) == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->output_file, 2);
			ft_putendl_fd(": Permission denied", 2);
			free(dir_path);
			restore_fds(prev_fds);
			return (0);
		}
	}
	free(dir_path);
	return (1);
}

int	setup_output_fd(t_cmd *cmd, int flags, int *prev_fds)
{
	cmd->output_fd = open(cmd->output_file, flags, 0644);
	if (cmd->output_fd == -1)
	{
		if (errno == EACCES)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->output_file, 2);
			ft_putendl_fd(": Permission denied", 2);
		}
		else
			perror("minishell: output redirection");
		restore_fds(prev_fds);
		return (0);
	}
	if (dup2(cmd->output_fd, STDOUT_FILENO) == -1)
	{
		perror("minishell: output redirection");
		close(cmd->output_fd);
		restore_fds(prev_fds);
		return (0);
	}
	close(cmd->output_fd);
	return (1);
}
