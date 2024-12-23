/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 15:20:19 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/02 13:36:28 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		g_signal_received = signo;
		if (isatty(STDIN_FILENO))
		{
			ft_putchar_fd('\n', STDERR_FILENO);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
		else
			ft_putchar_fd('\n', STDERR_FILENO);
	}
}

void	setup_signals(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	reset_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	setup_parent_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}
