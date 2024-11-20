/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 17:39:24 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/20 14:31:57 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"
/*
static int setup_input_redirection(t_cmd *cmd)
{
    int fd;

    if (cmd->input_file)
    {
        fd = open(cmd->input_file, O_RDONLY);
        if (fd == -1)
        {
            perror("minishell: input redirection");
            return (0);
        }
        if (dup2(fd, STDIN_FILENO) == -1)
        {
            perror("minishell: input redirection");
            close(fd);
            return (0);
        }
        close(fd);
    }
    return (1);
}

static int setup_output_redirection(t_cmd *cmd)
{
    int        fd;
    int        flags;

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
        if (dup2(fd, STDOUT_FILENO) == -1)
        {
            perror("minishell: output redirection");
            close(fd);
            return (0);
        }
        close(fd);
    }
    return (1);
}
*/

int setup_redirections(t_cmd *cmd)
{
    int     prev_in_fd;
    int     prev_out_fd;
    
    // Save previous fds in case we need to restore them on error
    prev_in_fd = dup(STDIN_FILENO);
    prev_out_fd = dup(STDOUT_FILENO);
    
    if (prev_in_fd == -1 || prev_out_fd == -1)
    {
        perror("minishell: dup");
        return (0);
    }

    // Handle input redirection
    if (cmd->input_file)
    {
        if (access(cmd->input_file, F_OK) == -1)
        {
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(cmd->input_file, 2);
            ft_putendl_fd(": No such file or directory", 2);
            close(prev_in_fd);
            close(prev_out_fd);
            return (0);
        }
        if (access(cmd->input_file, R_OK) == -1)
        {
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(cmd->input_file, 2);
            ft_putendl_fd(": Permission denied", 2);
            close(prev_in_fd);
            close(prev_out_fd);
            return (0);
        }
        cmd->input_fd = open(cmd->input_file, O_RDONLY);
        if (cmd->input_fd == -1 || dup2(cmd->input_fd, STDIN_FILENO) == -1)
        {
            perror("minishell: input redirection");
            close(prev_in_fd);
            close(prev_out_fd);
            return (0);
        }
        close(cmd->input_fd);
    }

    // Handle output redirection
    if (cmd->output_file)
    {
        int flags = cmd->append_output ? 
            (O_WRONLY | O_CREAT | O_APPEND) : 
            (O_WRONLY | O_CREAT | O_TRUNC);

        // Check directory permission first
        char *dir_path = ft_strdup(cmd->output_file);
        char *last_slash = ft_strrchr(dir_path, '/');
        if (last_slash)
        {
            *last_slash = '\0';
            if (access(dir_path, W_OK) == -1)
            {
                ft_putstr_fd("minishell: ", 2);
                ft_putstr_fd(cmd->output_file, 2);
                ft_putendl_fd(": Permission denied", 2);
                free(dir_path);
                // Restore original file descriptors
                dup2(prev_in_fd, STDIN_FILENO);
                dup2(prev_out_fd, STDOUT_FILENO);
                close(prev_in_fd);
                close(prev_out_fd);
                return (0);
            }
        }
        free(dir_path);

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
            // Restore original file descriptors
            dup2(prev_in_fd, STDIN_FILENO);
            dup2(prev_out_fd, STDOUT_FILENO);
            close(prev_in_fd);
            close(prev_out_fd);
            return (0);
        }
        if (dup2(cmd->output_fd, STDOUT_FILENO) == -1)
        {
            perror("minishell: output redirection");
            close(cmd->output_fd);
            // Restore original file descriptors
            dup2(prev_in_fd, STDIN_FILENO);
            dup2(prev_out_fd, STDOUT_FILENO);
            close(prev_in_fd);
            close(prev_out_fd);
            return (0);
        }
        close(cmd->output_fd);
    }

    close(prev_in_fd);
    close(prev_out_fd);
    return (1);
}