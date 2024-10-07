/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:42:22 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/07 14:19:58 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

t_parse_error	ft_handle_parse_error(t_parse_error error)
{
	if (error == PARSE_ERROR_SYNTAX)
		ft_putstr_fd("minishell: syntax error\n", 2);
	else if (error == PARSE_ERROR_MALLOC)
		ft_putstr_fd("minishell: malloc error\n", 2);
	else if (error == PARSE_ERROR_PIPE)
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
	return (error);
}

