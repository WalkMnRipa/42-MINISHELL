/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_cleanup.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 20:11:10 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/05 20:15:11 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

void free_cmd(t_cmd *cmd)
{
    char **args;

    if (cmd)
    {
        args = cmd->args;
        while (*args)
        {
            free(*args);
            args++;
        }
        free(cmd->args);
        if (cmd->input_fd != STDIN_FILENO)
            close(cmd->input_fd);
        if (cmd->output_fd != STDOUT_FILENO)
            close(cmd->output_fd);
        free(cmd);
    }
}

void cleanup(t_env *env, t_cmd *cmd)
{
    t_cmd *next;

    free_env(env);
    while (cmd)
    {
        next = cmd->next;
        free_cmd(cmd);
        cmd = next;
    }
}

void error_exit_message(t_env *env, t_cmd *cmd, const char *message)
{
    if (message)
    {
        if (errno)
            perror(message);
        else
            fprintf(stderr, "%s\n", message);
    }
    cleanup(env, cmd);
    exit(EXIT_FAILURE);
}
