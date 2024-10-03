/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
<<<<<<< HEAD
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 16:09:28 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/02 16:09:29 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

=======
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 18:27:00 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/01 20:03:26 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_shell	shell;

	(void)argc;
	(void)argv;
	ft_memset(&shell, 0, sizeof(t_shell));
	initialize_shell(&shell, envp);
	setup_signals();
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
			break ;
		if (*input)
		{
			add_history(input);
			execute_command(&shell, input);
		}
		free(input);
	}
	cleanup_shell(&shell);
	return (shell.exit_status);
}
>>>>>>> ggaribot
