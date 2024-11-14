/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:37:35 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/13 16:37:52 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

void update_exit_status(t_cmd *cmd, int status)
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

void set_last_exit_status(t_env *env, int status)
{
    env->last_exit_status = status;
}
