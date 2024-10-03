/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 19:15:03 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/01 19:58:27 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

void	cleanup_shell(t_shell *shell)
{
	if (shell->env)
		free_string_array(shell->env);
	if (shell->cmd_paths)
		free_string_array(shell->cmd_paths);
	if (shell->cmd_args)
		free_string_array(shell->cmd_args);
	if (shell->cmd)
		free(shell->cmd);
}

void	error_exit(t_shell *shell, char *error_message)
{
	if (error_message)
		ft_putendl_fd(error_message, 2);
	if (shell)
		cleanup_shell(shell);
	exit(1);
}