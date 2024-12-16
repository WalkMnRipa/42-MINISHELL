/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_cleanup.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 20:11:10 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/16 23:48:21 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	free_cmd_args(char **args)
{
	char	**tmp;

	if (!args)
		return ;
	tmp = args;
	while (*tmp)
	{
		free(*tmp);
		tmp++;
	}
	free(args);
}

void	free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->args)
		free_cmd_args(cmd->args);
	if (cmd->input_file)
		free(cmd->input_file);
	if (cmd->output_file)
		free(cmd->output_file);
	if (cmd->heredocs)
		free_heredocs(cmd->heredocs);
	if (cmd->input_fd != STDIN_FILENO)
		close(cmd->input_fd);
	if (cmd->output_fd != STDOUT_FILENO)
		close(cmd->output_fd);
	free(cmd);
}

void	cleanup(t_env *env, t_cmd *cmd)
{
	t_cmd	*next;

	free_env(env);
	while (cmd)
	{
		next = cmd->next;
		free_cmd(cmd);
		cmd = next;
	}
}

void	error_exit_message(t_env *env, t_cmd *cmd, char *message)
{
	if (message)
		ft_putendl_fd(message, 2);
	cleanup(env, cmd);
	exit(EXIT_FAILURE);
}

