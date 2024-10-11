/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:00:00 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/11 15:40:14 by ggaribot         ###   ########.fr       */
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

t_cmd	*tokens_to_cmd(t_token *tokens)
{
	t_cmd	*cmd;
	int		arg_count;
	t_token	*current;
	int		i;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	arg_count = 0;
	current = tokens;
	while (current && current->type != TOKEN_PIPE)
	{
		arg_count++;
		current = current->next;
	}
	cmd->args = malloc(sizeof(char *) * (arg_count + 1));
	if (!cmd->args)
	{
		free(cmd);
		return (NULL);
	}
	i = 0;
	current = tokens;
	while (current && current->type != TOKEN_PIPE)
	{
		cmd->args[i] = ft_strdup(current->value);
		i++;
		current = current->next;
	}
	cmd->args[i] = NULL;
	cmd->input_fd = STDIN_FILENO;
	cmd->output_fd = STDOUT_FILENO;
	cmd->next = NULL;
	return (cmd);
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
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
			break ; // EOF (Ctrl+D)
		if (*input)
			add_history(input);
		tokens = ft_tokenizer(input);
		if (!tokens)
		{
			free(input);
			continue ;
		}
		cmd = tokens_to_cmd(tokens);
		// This function needs to be implemented by the parsing team
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
