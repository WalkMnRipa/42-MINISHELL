/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:00:00 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/21 15:03:20 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"
#include "../includes/parsing.h"
#include <readline/history.h>
#include <readline/readline.h>

int				g_signal_received = 0;

static void	process_input(char *input, t_env **env)
{
	t_token	*tokens;
	t_cmd	*cmd;

	add_history(input);
	tokens = tokenizer(input);
	if (tokens)
	{
		cmd = group_tokens_into_commands(tokens, *env);
		if (cmd)
		{
			execute_command(cmd, env);
			free_cmd_list(cmd);
		}
		free_tokens(tokens);
	}
}

static int	shell_loop(t_env *env, int stdin_backup)
{
	char	*input;
	t_token	*tokens;
	t_cmd	*cmd;

	while (1)
	{
		g_signal_received = 0;
		dup2(stdin_backup, STDIN_FILENO);
		input = readline("minishell> ");
		if (!input)
		{
			if (isatty(STDIN_FILENO))
				printf("\nexit minishell\n");
			break ;
		}
		if (*input)
		{
			add_history(input);
			tokens = tokenizer(input);
			if (tokens)
			{
				cmd = group_tokens_into_commands(tokens, env);
				if (cmd)
				{
					execute_command(cmd, &env);
					free_cmd_list(cmd);
				}
				free_tokens(tokens);
			}
		}
		ft_putendl_fd("exit", STDOUT_FILENO);
		break ;
	}
	if (*input)
		process_input(input, &env);
	free(input);
	return (0);
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
	exit_status = shell_loop(env, stdin_backup);
	close(stdin_backup);
	cleanup(env, NULL);
	return (exit_status);
}
