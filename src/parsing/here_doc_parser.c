/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_parser.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 15:30:56 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/25 16:01:02 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

int	validate_heredoc_syntax(t_token *token)
{
	if (!token || !token->next || token->type != TOKEN_HERE_DOC)
		return (0);
	if (token->next->type != TOKEN_WORD)
	{
		ft_putendl_fd(ERR_UNEXPECTED_NL, 2);
		return (0);
	}
	return (1);
}

static char	*extract_delimiter(t_token *token)
{
	if (!token || !token->next || !token->next->value)
		return (NULL);
	return (ft_strdup(token->next->value));
}

int	handle_heredoc_token(t_token *token, t_cmd *cmd)
{
	char *delimiter;

	if (!validate_heredoc_syntax(token))
		return (0);
	delimiter = extract_delimiter(token);
	if (!delimiter)
		return (0);
	if (cmd->input_file)
		free(cmd->input_file);
	cmd->input_file = delimiter;
	cmd->input_fd = -2;
	return (1);
}