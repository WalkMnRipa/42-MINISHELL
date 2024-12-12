/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:00:00 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/12 17:04:31 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int			g_signal_received = 0;

static int	handle_input(char *input, t_env **env, int *exit_status)
{
	t_token	*tokens;
	t_cmd	*cmd;

	if (!input || !*input)
		return (0);
	add_history(input);
	tokens = tokenizer(input, *env);
	if (!tokens)
		return (0);
	cmd = group_tokens_into_commands(tokens, *env);
	if (cmd)
	{
		execute_command(cmd, env);
		if (exit_status)
			*exit_status = cmd->exit_status;
		if (env && *env)
			(*env)->last_exit_status = cmd->exit_status;
		free_cmd_list(cmd);
	}
	free_tokens(tokens);
	return (0);
}

int	shell_loop(t_env **env)
{
	int		exit_status;
	char	*input;

	exit_status = 0;
	while (1)
	{
		g_signal_received = 0;
		input = readline("minishell> ");
		if (!input)
		{
			if (isatty(STDIN_FILENO))
				ft_putendl_fd("exit", STDOUT_FILENO);
			break ;
		}
		handle_input(input, env, &exit_status);
		free(input);
	}
	return (exit_status);
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;
	int		stdin_backup;
	int		exit_status;

	(void)argc;
	(void)argv;
	env = initialize_shell(envp);
	if (!env)
		return (1);
	stdin_backup = dup(STDIN_FILENO);
	if (stdin_backup == -1)
	{
		perror("minishell: dup failed");
		cleanup(env, NULL);
		return (1);
	}
	setup_signals();
	exit_status = shell_loop(&env);
	close(stdin_backup);
	cleanup(env, NULL);
	return (exit_status);
}
