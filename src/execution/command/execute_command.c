/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 20:14:16 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/17 14:16:52 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/execution.h"

char	*find_command_path(const char *command, t_env *env)
{
	char	*path;
	char	*path_copy;
	char	*dir;
	char	*result;

	if (is_direct_executable(command))
		return (ft_strdup(command));
	path = get_env_value(env, "PATH");
	if (!path)
		return (NULL);
	path_copy = ft_strdup(path);
	if (!path_copy)
		return (NULL);
	dir = ft_strtok(path_copy, ":");
	while (dir != NULL)
	{
		result = try_path(dir, command);
		if (result)
		{
			free(path_copy);
			return (result);
		}
		dir = ft_strtok(NULL, ":");
	}
	return (free(path_copy), NULL);
}

/*static void	handle_command_not_found(t_cmd *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd->args[0], 2);
	ft_putendl_fd(": command not found", 2);
	cmd->exit_status = 127;
}*/

/*static void	wait_for_child(pid_t pid, t_cmd *cmd)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		cmd->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		cmd->exit_status = 128 + WTERMSIG(status);
}*/

/*static void	handle_external_command(t_cmd *cmd, t_env **env)
{
	char	*command_path;
	pid_t	pid;

	command_path = find_command_path(cmd->args[0], *env);
	if (!command_path)
	{
		handle_command_not_found(cmd);
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		cmd->exit_status = 1;
	}
	else if (pid == 0)
	{
		execve(command_path, cmd->args, NULL);
		perror("minishell: execve");
		exit(EXIT_FAILURE);
	}
	else
		wait_for_child(pid, cmd);
	free(command_path);
}*/

void execute_command(t_cmd *cmd, t_env **env)
{
    dprintf(2, "execute_command: Command: %s\n", cmd->args[0]);
    dprintf(2, "  Actual input fd: %d, Actual output fd: %d\n", 
            fcntl(0, F_GETFL), fcntl(1, F_GETFL));
    dprintf(2, "  Command arguments: ");
    for (int i = 0; cmd->args[i] != NULL; i++)
    {
        dprintf(2, "%s ", cmd->args[i]);
    }
    dprintf(2, "\n");

    if (is_builtin(cmd->args[0]))
    {
        dprintf(2, "Executing builtin: %s\n", cmd->args[0]);
        execute_builtin(cmd, env);
    }
    else
    {
        dprintf(2, "Executing external command: %s\n", cmd->args[0]);
        char *command_path = find_command_path(cmd->args[0], *env);
        if (!command_path)
        {
            dprintf(2, "Command not found: %s\n", cmd->args[0]);
            cmd->exit_status = 127;
            return;
        }
        execve(command_path, cmd->args, NULL);
        dprintf(2, "execve failed for command: %s\n", cmd->args[0]);
        perror("execve");
        free(command_path);
        exit(EXIT_FAILURE);
    }
    dprintf(2, "Command execution finished: %s\n", cmd->args[0]);
}
