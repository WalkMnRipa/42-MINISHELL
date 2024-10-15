/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:00:00 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/15 19:05:23 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"
#include "../includes/parsing.h"
#include <readline/history.h>
#include <readline/readline.h>

int		g_signal_received = 0;

void	signal_handler(int signo)
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

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_token	*tokens;
	t_cmd	*cmd;
	t_env	*env;

	(void)argc;
	(void)argv;
	env = init_env(envp);
	if (!env)
		return (1);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
			break ;
		if (*input)
			add_history(input);
		tokens = tokenizer(input);
		if (!tokens)
		{
			free(input);
			continue ;
		}
		cmd = group_tokens_into_commands(tokens);
		if (!cmd)
		{
			free_tokens(tokens);
			free(input);
			continue ;
		}
		execute_command(cmd, &env);
		free_tokens(tokens);
		free_cmd(cmd);
		free(input);
	}
	cleanup(env, NULL);
	return (0);
}
