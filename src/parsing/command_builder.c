/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 16:27:46 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/11 21:49:38 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

static t_cmd	*create_new_command(void)
{
	t_cmd	*new_cmd;

	new_cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	new_cmd->args = NULL;
	new_cmd->input_fd = STDIN_FILENO;
	new_cmd->output_fd = STDOUT_FILENO;
	new_cmd->exit_status = 0;
	new_cmd->next = NULL;
	return (new_cmd);
}

static int	add_argument(t_cmd *cmd, char *arg)
{
	int		args_count;
	char	**new_args;
	int		i;

	args_count = 0;
	while (cmd->args && cmd->args[args_count])
		args_count++;
	new_args = (char **)malloc(sizeof(char *) * (args_count + 2));
	if (!new_args)
		return (0);
	i = 0;
	while (cmd->args && cmd->args[i])
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[i] = ft_strdup(arg);
	if (!new_args[i])
		return (free(new_args), 0);
	new_args[i + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
	return (1);
}

static int	handle_redirection(t_token **token, t_cmd *cmd)
{
	int	fd;

	if ((*token)->type == TOKEN_REDIR_INPUT)
		fd = open((*token)->next->value, O_RDONLY);
	else if ((*token)->type == TOKEN_REDIR_OUTPUT)
		fd = open((*token)->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if ((*token)->type == TOKEN_REDIR_APPEND)
		fd = open((*token)->next->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		return (0);
	if (fd == -1)
		return (0);
	if ((*token)->type == TOKEN_REDIR_INPUT)
		cmd->input_fd = fd;
	else
		cmd->output_fd = fd;
	*token = (*token)->next;
	return (1);
}

static int	process_token(t_token **token, t_cmd **current, t_cmd **head)
{
	if ((*token)->type == TOKEN_WORD)
	{
		if (!add_argument(*current, (*token)->value))
			return (0);
	}
	else if ((*token)->type == TOKEN_PIPE)
	{
		(*current)->next = create_new_command();
		if (!(*current)->next)
			return (0);
		*current = (*current)->next;
	}
	else if ((*token)->type == TOKEN_REDIR_INPUT
		|| (*token)->type == TOKEN_REDIR_OUTPUT
		|| (*token)->type == TOKEN_REDIR_APPEND)
	{
		if (!handle_redirection(token, *current))
			return (0);
	}
	return (1);
}

t_cmd	*group_tokens_into_commands(t_token *token_list)
{
	t_cmd	*head;
	t_cmd	*current;
	t_token	*token;

	head = NULL;
	current = NULL;
	token = token_list;
	while (token)
	{
		if (!current)
		{
			current = create_new_command();
			if (!current)
				return (NULL);
			if (!head)
				head = current;
		}
		if (!process_token(&token, &current, &head))
			return (NULL);
		token = token->next;
	}
	return (head);
}
