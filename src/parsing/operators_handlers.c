/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operators_handlers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 16:00:54 by jcohen            #+#    #+#             */
/*   Updated: 2024/11/18 15:31:44 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

int	modify_token_type(t_token *token, t_token_type new_type)
{
	if (!token)
		return (1);
	token->type = new_type;
	return (0);
}

int	handle_pipe(t_token *current)
{
	if (!current->next)
	{
		ft_putendl_fd(ERR_UNEXPECTED_PIPE, 2);
		return (1);
	}
	return (modify_token_type(current, TOKEN_PIPE));
}

int	handle_redir_input(t_token *current)
{
	if (!current->next)
	{
		ft_putendl_fd(ERR_UNEXPECTED_NL, 2);
		return (1);
	}
	return (modify_token_type(current, TOKEN_REDIR_INPUT));
}

int	handle_redir_output(t_token *current)
{
	if (!current->next)
	{
		ft_putendl_fd(ERR_UNEXPECTED_NL, 2);
		return (1);
	}
	return (modify_token_type(current, TOKEN_REDIR_OUTPUT));
}

int	handle_redir_append(t_token *current)
{
	if (!current->next)
	{
		ft_putendl_fd(ERR_UNEXPECTED_NL, 2);
		return (1);
	}
	return (modify_token_type(current, TOKEN_REDIR_APPEND));
}
