/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:37:14 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/17 14:07:54 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"
#include <fcntl.h>

static void execute_piped_command(t_cmd *cmd, t_env **env, int input_fd, int output_fd)
{
    pid_t pid;

    dprintf(2, "Setting up command: %s\n", cmd->args[0]);
    pid = fork();
    if (pid == -1)
        error_exit_message(*env, cmd, "Fork failed");
    else if (pid == 0)
    {
        // Child process
        dprintf(2, "Child process for command: %s\n", cmd->args[0]);
        if (input_fd != STDIN_FILENO)
        {
            dprintf(2, "Redirecting input for %s from fd %d\n", cmd->args[0], input_fd);
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }
        if (output_fd != STDOUT_FILENO)
        {
            dprintf(2, "Redirecting output for %s to fd %d\n", cmd->args[0], output_fd);
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }
        
        dprintf(2, "Executing command: %s, input_fd: %d, output_fd: %d\n", cmd->args[0], input_fd, output_fd);
        
        execute_command(cmd, env);
        dprintf(2, "Command %s finished execution\n", cmd->args[0]);
        exit(cmd->exit_status);
    }
    else
    {
        dprintf(2, "Parent process: command %s forked with PID %d\n", cmd->args[0], pid);
    }
}

void execute_pipeline(t_cmd *cmd_list, t_env **env)
{
    int pipefd[2];
    int prev_pipe = STDIN_FILENO;
    t_cmd *current = cmd_list;
    int cmd_count = 0;

    dprintf(2, "Starting pipeline execution\n");

    while (current)
    {
        cmd_count++;
        dprintf(2, "Processing command %d: %s\n", cmd_count, current->args[0]);
        
        if (current->next)
        {
            if (pipe(pipefd) == -1)
                error_exit_message(*env, current, "Pipe creation failed");
            
            dprintf(2, "Pipe created for command %d: read_fd: %d, write_fd: %d\n", cmd_count, pipefd[0], pipefd[1]);
        }

        execute_piped_command(current, env, prev_pipe, 
                              current->next ? pipefd[1] : STDOUT_FILENO);

        if (prev_pipe != STDIN_FILENO)
        {
            dprintf(2, "Closing previous pipe read end: %d\n", prev_pipe);
            close(prev_pipe);
        }

        if (current->next)
        {
            dprintf(2, "Closing current pipe write end: %d\n", pipefd[1]);
            close(pipefd[1]);
            prev_pipe = pipefd[0];
        }

        current = current->next;
    }

    dprintf(2, "Waiting for child processes to finish\n");
    while (wait(NULL) > 0)
    {
        dprintf(2, "Child process finished\n");
    }
    dprintf(2, "Pipeline execution completed\n");
}
