/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:00:00 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/17 14:47:42 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"
#include "../includes/parsing.h"
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>

int		g_signal_received = 0;

void	signal_handler(int signo)
{
	g_signal_received = signo;
	write(STDERR_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int main(int argc, char **argv, char **envp)
{
    char    *input;
    t_token *tokens;
    t_cmd   *cmd;
    t_env   *env;
    int     stdin_backup;

    (void)argc;
    (void)argv;
    env = init_env(envp);
    if (!env)
        return (1);
    signal(SIGINT, signal_handler);
    signal(SIGQUIT, SIG_IGN);
    stdin_backup = dup(STDIN_FILENO);
    while (1)
    {
        g_signal_received = 0;
        dup2(stdin_backup, STDIN_FILENO);
        input = readline("minishell> ");
        if (!input)
        {
            if (isatty(STDIN_FILENO))
                printf("\nExiting minishell\n");
            break;
        }
        if (*input)
        {
            add_history(input);
            tokens = tokenizer(input);
            if (tokens)
            {
                cmd = group_tokens_into_commands(tokens);
                if (cmd)
                {
                    dprintf(2, "Debug: Command structure created\n");
                    if (cmd->next) // If there's more than one command, it's a pipeline
                    {
                        dprintf(2, "Debug: Executing pipeline\n");
                        execute_pipeline(cmd, &env);
                    }
                    else
                    {
                        dprintf(2, "Debug: Executing single command\n");
                        execute_command(cmd, &env);
                    }
                    free_cmd_list(cmd);
                }
                free_tokens(tokens);
            }
        }
        free(input);
    }
    close(stdin_backup);
    cleanup(env, NULL);
    return (0);
}
