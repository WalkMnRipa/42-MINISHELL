/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 15:20:19 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/21 16:55:06 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"
#include "../../includes/parsing.h"
#include <readline/readline.h>
#include <readline/history.h>

static void signal_handler(int signo)
{
    if (signo == SIGINT)
    {
        g_signal_received = signo;
        if (isatty(STDIN_FILENO)) // Interactive mode
        {
            ft_putchar_fd('\n', STDERR_FILENO); // Ensure newline
            rl_on_new_line();
            rl_replace_line("", 0);
            rl_redisplay();
        }
        else
        {
            ft_putchar_fd('\n', STDERR_FILENO); // Explicit newline for commands like cat
        }
    }
}

void setup_signals(void)
{
    signal(SIGINT, signal_handler);
    signal(SIGQUIT, SIG_IGN);
}

void reset_signals(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}

void setup_parent_signals(void)
{
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}
