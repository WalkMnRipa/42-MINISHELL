/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:00:00 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/11 22:30:19 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"
#include "../includes/parsing.h"
#include <readline/history.h>
#include <readline/readline.h>

int			g_signal_received = 0;

static void	signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		g_signal_received = signo;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

static void	setup_signals(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

static void	process_input(char *input, t_env **env)
{
	t_token	*tokens;
	t_cmd	*cmd_list;

	tokens = tokenizer(input);
	if (!tokens)
		return ;
	cmd_list = group_tokens_into_commands(tokens);
	if (!cmd_list)
	{
		free_tokens(tokens);
		return ;
	}
	execute_commands(cmd_list, env);
	free_tokens(tokens);
	free_cmd_list(cmd_list);
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_env	*env;

	(void)argc;
	(void)argv;
	env = init_env(envp);
	if (!env)
		return (1);
	setup_signals();
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
			break ;
		if (*input)
		{
			add_history(input);
			process_input(input, &env);
		}
		free(input);
	}
	cleanup(env, NULL);
	return (0);
}
