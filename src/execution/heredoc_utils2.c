/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 23:27:29 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/01 23:27:57 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_heredoc_signal(int sig)
{
	if (sig == SIGINT)
	{
		g_signal_received = 1;
		write(STDERR_FILENO, "\n", 1);
		close(STDIN_FILENO);
	}
}

void	setup_heredoc_signals(void)
{
	signal(SIGINT, handle_heredoc_signal);
	signal(SIGQUIT, SIG_IGN);
}
