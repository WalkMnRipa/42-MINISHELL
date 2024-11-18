/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_builder.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 16:27:46 by jcohen            #+#    #+#             */
/*   Updated: 2024/11/18 15:31:41 by ggaribot         ###   ########.fr       */
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
	new_cmd->input_file = NULL;
	new_cmd->output_file = NULL;
	new_cmd->append_output = 0;
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

	if (!arg || !*arg) // Skip empty arguments
		return (1);
	args_count = 0;
	while (cmd->args && cmd->args[args_count])
		args_count++;
	new_args = malloc(sizeof(char *) * (args_count + 2));
	if (!new_args)
		return (0);
	i = 0;
	while (i < args_count)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[args_count] = ft_strdup(arg);
	if (!new_args[args_count])
	{
		free(new_args);
		return (0);
	}
	new_args[args_count + 1] = NULL;
	free(cmd->args); // Free old args array but not its contents
	cmd->args = new_args;
	return (1);
}

static int	handle_redirection(t_token **token, t_cmd *cmd)
{
	if ((*token)->type == TOKEN_REDIR_INPUT)
	{
		cmd->input_file = ft_strdup((*token)->next->value);
		if (!cmd->input_file)
			return (0);
	}
	else if ((*token)->type == TOKEN_REDIR_OUTPUT)
	{
		cmd->output_file = ft_strdup((*token)->next->value);
		if (!cmd->output_file)
			return (0);
		cmd->append_output = 0;
	}
	else if ((*token)->type == TOKEN_REDIR_APPEND)
	{
		cmd->output_file = ft_strdup((*token)->next->value);
		if (!cmd->output_file)
			return (0);
		cmd->append_output = 1;
	}
	*token = (*token)->next;
	return (1);
}

static int	process_token(t_token **token, t_cmd **current, t_env *env)
{
	if (!*token)
		return (0);
	if ((*token)->type == TOKEN_WORD)
	{
		if (!(*token)->value || !*(*token)->value) // Skip empty tokens
			return (1);
		return (add_argument(*current, (*token)->value));
	}
	if ((*token)->type == TOKEN_PIPE)
	{
		(*current)->next = create_new_command();
		if (!(*current)->next)
			return (0);
		*current = (*current)->next;
		return (1);
	}
	if ((*token)->type == TOKEN_REDIR_INPUT
		|| (*token)->type == TOKEN_REDIR_OUTPUT
		|| (*token)->type == TOKEN_REDIR_APPEND)
		return (handle_redirection(token, *current));
	if ((*token)->type == TOKEN_HERE_DOC)
	{
		if (!(*token)->next || handle_heredoc(*current, (*token)->next->value,
				env) != 0)
			return (0);
		*token = (*token)->next;
	}
	return (1);
}

t_cmd	*group_tokens_into_commands(t_token *token_list, t_env *env)
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
				return (free_cmd_list(head), NULL);
			if (!head)
				head = current;
		}
		if (!process_token(&token, &current, env))
			return (free_cmd_list(head), NULL);
		token = token->next;
	}
	return (head);
}
