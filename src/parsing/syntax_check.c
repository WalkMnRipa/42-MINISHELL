/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 19:00:26 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/25 04:49:32 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

void	print_syntax_error(char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(token, 2);
	ft_putendl_fd("'", 2);
}

static int	check_pipe_syntax(t_token *tokens)
{
	if (tokens->type == TOKEN_PIPE)
	{
		print_syntax_error("|");
		return (0);
	}
	while (tokens)
	{
		if (tokens->type == TOKEN_PIPE)
		{
			if (!tokens->next || tokens->next->type == TOKEN_PIPE)
			{
				print_syntax_error(tokens->next ? "|" : "newline");
				return (0);
			}
		}
		tokens = tokens->next;
	}
	return (1);
}

static int	check_redirection_syntax(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type >= TOKEN_REDIR_IN && tokens->type <= TOKEN_HEREDOC)
		{
			if (!tokens->next || tokens->next->type != TOKEN_WORD)
			{
				print_syntax_error(tokens->next ? tokens->next->value : "newline");
				return (0);
			}
		}
		tokens = tokens->next;
	}
	return (1);
}

int	check_syntax_errors(t_token *tokens)
{
	if (!tokens)
		return (1);
	if (!check_pipe_syntax(tokens))
		return (0);
	if (!check_redirection_syntax(tokens))
		return (0);
	return (1);
}
