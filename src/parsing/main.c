/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:21:16 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/07 13:21:40 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// minishell.c
int	main(int argc, char **argv, char **envp)
{
	t_shell_data	shell_data;
	char			*input;
	t_token			*tokens;

	initialize_shell_data(&shell_data, envp);
	setup_signals();
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
			break ;
		add_history(input);
		tokens = ft_tokenizer(input);
		shell_data.commands = parse_commands(tokens, &shell_data);
		execute_commands(shell_data.commands, &shell_data);
		cleanup_iteration(&shell_data);
		free(input);
	}
	cleanup_shell(&shell_data);
	return (0);
}
