/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 18:28:10 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/01 20:03:10 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

static int	is_builtin(char *cmd)
{
	return (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0);
}

static void	execute_builtin(t_shell *shell, char **args)
{
	if (ft_strcmp(args[0], "echo") == 0)
		builtin_echo(args);
	else if (ft_strcmp(args[0], "cd") == 0)
		builtin_cd(shell, args);
	else if (ft_strcmp(args[0], "pwd") == 0)
		builtin_pwd();
	else if (ft_strcmp(args[0], "export") == 0)
		builtin_export(shell, args);
	else if (ft_strcmp(args[0], "unset") == 0)
		builtin_unset(shell, args);
	else if (ft_strcmp(args[0], "env") == 0)
		builtin_env(shell);
	else if (ft_strcmp(args[0], "exit") == 0)
		builtin_exit(shell, args);
}

static void	execute_external_command(t_shell *shell, char **args)
{
	pid_t	pid;
	int		status;

	shell->cmd = get_cmd(shell->cmd_paths, args[0]);
	if (!shell->cmd)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putendl_fd(args[0], 2);
		shell->exit_status = 127;
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (execve(shell->cmd, args, shell->env) == -1)
			error_exit(shell, "Error: Command execution failed");
	}
	else if (pid < 0)
		error_exit(shell, "Error: Fork failed");
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			shell->exit_status = 128 + WTERMSIG(status);
	}
	free(shell->cmd);
}

void	execute_command(t_shell *shell, char *input)
{
	char	*expanded_input;
	int		i;

	expanded_input = expand_env_vars(shell, input);
	shell->cmd_args = ft_split(expanded_input, ' ');
	free(expanded_input);
	if (!shell->cmd_args || !shell->cmd_args[0])
	{
		free_string_array(shell->cmd_args);
		return ;
	}
	i = 0;
	while (shell->cmd_args[i])
	{
		shell->cmd_args[i] = expand_env_vars(shell, shell->cmd_args[i]);
		i++;
	}
	if (is_builtin(shell->cmd_args[0]))
		execute_builtin(shell, shell->cmd_args);
	else
		execute_external_command(shell, shell->cmd_args);
	free_string_array(shell->cmd_args);
}
