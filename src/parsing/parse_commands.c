/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:07:39 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/07 14:46:10 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

t_command	*create_command(void)
{
	t_command	*command;

	command = (t_command *)malloc(sizeof(t_command));
	if (!command)
		return (NULL);
	command->args = (char **)malloc(sizeof(char *));
	if (!command->args)
	{
		free(command);
		return (NULL);
	}
	command->args[0] = NULL;
	command->redirects = NULL;
	command->next = NULL;
	command->pipe_out = 0;
	return (command);
	return (command);
}

t_parse_error	handle_pipe(t_command **current, t_command **head)
{
	t_command	*new_command;

	if (!*current || !(*current)->args[0])
		return (ft_handle_parse_error(PARSE_ERROR_PIPE));
	new_command = create_command();
	if (!new_command)
		return (ft_handle_parse_error(PARSE_ERROR_MALLOC));
	(*current)->pipe_out = 1;
	(*current)->next = new_command;
	*current = new_command;
	if (!*head)
		*head = new_command;
	return (PARSE_ERROR_NONE);
}

t_parse_error	add_argument(t_command *command, char *argument)
{
	char	**new_args;
	int		i;

	i = 0;
	while (command->args[i])
		i++;
	new_args = (char **)malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		return (ft_handle_parse_error(PARSE_ERROR_MALLOC));
	ft_copy_array(command->args, new_args);
	new_args[i] = argument;
	if (!new_args[i])
	{
		free(new_args);
		return (ft_handle_parse_error(PARSE_ERROR_MALLOC));
	}
	new_args[i + 1] = NULL;
	free(command->args);
	command->args = new_args;
	return (PARSE_ERROR_NONE);
}

t_parse_error	add_redirect(t_command *command, t_token *token)
{
	t_redirect	*new_redirect;
	t_redirect	*current;

	if (!token->next || token->next->type != TOKEN_WORD)
		return (ft_handle_parse_error(PARSE_ERROR_REDIR));
	new_redirect = (t_redirect *)malloc(sizeof(t_redirect));
	if (!new_redirect)
		return (ft_handle_parse_error(PARSE_ERROR_MALLOC));
	new_redirect->type = token->type;
	new_redirect->file = ft_strdup(token->next->value);
	if (!new_redirect->file)
	{
		free(new_redirect);
		return (ft_handle_parse_error(PARSE_ERROR_MALLOC));
	}
	new_redirect->next = NULL;
	if (!command->redirects)
		command->redirects = new_redirect;
	else
	{
		current = command->redirects;
		while (current->next)
			current = current->next;
		current->next = new_redirect;
	}
	return (PARSE_ERROR_NONE);
}

t_command	*parse_commands(t_token *tokens)
{
	t_command		*head;
	t_command		*current_command;
	t_parse_error	error;

	head = NULL;
	current_command = NULL;
	while (tokens)
	{
		if (tokens->type == TOKEN_PIPE)
		{
			error = handle_pipe(&current_command, &head);
			if (error != PARSE_ERROR_NONE)
				return (NULL);
		}
		else if (tokens->type == TOKEN_REDIR_INPUT
			|| tokens->type == TOKEN_REDIR_OUTPUT
			|| tokens->type == TOKEN_REDIR_APPEND
			|| tokens->type == TOKEN_HERE_DOC)
		{
			if (!current_command)
				current_command = create_command();
			error = add_redirect(current_command, tokens);
			if (error != PARSE_ERROR_NONE)
				return (NULL);
			tokens = tokens->next; // Skip the file name or delimiter token
		}
		else if (tokens->type == TOKEN_WORD)
		{
			if (!current_command)
				current_command = create_command();
			error = add_argument(current_command, tokens->value);
			if (error != PARSE_ERROR_NONE)
				return (NULL);
		}
		tokens = tokens->next;
	}
	if (!head)
		head = current_command;
	return (head);
}
