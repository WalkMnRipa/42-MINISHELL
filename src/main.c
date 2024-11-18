/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:00:00 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/18 15:31:27 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"
#include "../includes/parsing.h"
#include <readline/history.h>
#include <readline/readline.h>

int				g_signal_received = 0;

static int	handle_input(char *input, t_env **env, int *exit_status)
{
	t_token	*tokens;
	t_cmd	*cmd;

	if (!*input)
		return (0);
	add_history(input);
	tokens = tokenizer(input, *env);
	if (!tokens)
		return (0);
	cmd = group_tokens_into_commands(tokens, *env);
	if (cmd)
	{
		execute_command(cmd, env);
		*exit_status = cmd->exit_status;
		(*env)->last_exit_status = cmd->exit_status;
		free_cmd_list(cmd);
	}
	free_tokens(tokens);
	return (0);
}

static int	shell_loop(t_env **env, int stdin_backup)
{
	char	*input;
	int		exit_status;

	exit_status = 0;
	while (1)
	{
		g_signal_received = 0;
		dup2(stdin_backup, STDIN_FILENO);
		input = readline("minishell> ");
		if (!input)
		{
			if (isatty(STDIN_FILENO))
				ft_printf("exit\n");
			break ;
		}
		handle_input(input, env, &exit_status);
		free(input);
	}
	return (exit_status);
}

static t_env	*initialize_shell(char **envp)
{
	t_env	*env;

	env = init_env(envp);
	if (!env)
		return (NULL);
	setup_signals();
	return (env);
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
	exit_status = shell_loop(&env, stdin_backup);
	close(stdin_backup);
	cleanup(env, NULL);
	return (exit_status);
}
