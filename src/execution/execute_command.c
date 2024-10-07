/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 20:14:16 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/05 20:14:32 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

char *find_command_path(const char *command, t_env *env)
{
    char *path;
    char *path_copy;
    char *dir;
    char *full_path;

    path = get_env_value(env, "PATH");
    path_copy = strdup(path);
    dir = strtok(path_copy, ":");
    while (dir != NULL)
    {
        full_path = malloc(strlen(dir) + strlen(command) + 2);
        sprintf(full_path, "%s/%s", dir, command);
        if (access(full_path, X_OK) == 0)
        {
            free(path_copy);
            return (full_path);
        }
        free(full_path);
        dir = strtok(NULL, ":");
    }
    free(path_copy);
    return (NULL);
}

static void execute_child_process(t_cmd *cmd, t_env *env)
{
    char *command_path;

    if (cmd->input_fd != STDIN_FILENO)
    {
        if (dup2(cmd->input_fd, STDIN_FILENO) == -1)
            error_exit_message(env, cmd, "dup2");
        close(cmd->input_fd);
    }
    if (cmd->output_fd != STDOUT_FILENO)
    {
        if (dup2(cmd->output_fd, STDOUT_FILENO) == -1)
            error_exit_message(env, cmd, "dup2");
        close(cmd->output_fd);
    }
    command_path = find_command_path(cmd->args[0], env);
    if (command_path)
    {
        execve(command_path, cmd->args, NULL);
        error_exit_message(env, cmd, "execve");
    }
    else
    {
        fprintf(stderr, "%s: command not found\n", cmd->args[0]);
        exit(127);
    }
}

void execute_command(t_cmd *cmd, t_env *env)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0)
        execute_child_process(cmd, env);
    else if (pid < 0)
        error_exit_message(env, cmd, "fork");
    else
    {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            exit(WEXITSTATUS(status));
    }
}
