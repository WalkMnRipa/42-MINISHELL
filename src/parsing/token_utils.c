/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 00:59:04 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/19 01:06:59 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

void	free_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
}

void	free_cmd_list(t_cmd *cmd)
{
	t_cmd	*current;
	t_cmd	*next;

	current = cmd;
	while (current)
	{
		next = current->next;
		if (current->args)
			free_string_array(current->args, -1);
		if (current->input_file)
			free(current->input_file);
		if (current->output_file)
			free(current->output_file);
		free(current);
		current = next;
	}
}

void	free_string_array(char **array, int count)
{
	int	i;

	if (!array)
		return ;
	if (count == -1)
	{
		i = 0;
		while (array[i])
			free(array[i++]);
	}
	else
	{
		i = 0;
		while (i < count)
			free(array[i++]);
	}
	free(array);
}

/////////////////////////////////////////

static t_cmd	*init_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->input_fd = STDIN_FILENO;
	cmd->output_fd = STDOUT_FILENO;
	cmd->append_output = 0;
	cmd->next = NULL;
	cmd->exit_status = 0;
	return (cmd);
}

static int	count_args(t_token *start, t_token *end)
{
	int		count;
	t_token	*current;

	count = 0;
	current = start;
	while (current && current != end)
	{
		if (current->type == TOKEN_WORD)
			count++;
		current = current->next;
	}
	return (count);
}

static char	**create_args_array(t_token *start, t_token *end, int arg_count)
{
	char	**args;
	t_token	*current;
	int		i;

	args = malloc(sizeof(char *) * (arg_count + 1));
	if (!args)
		return (NULL);
	current = start;
	i = 0;
	while (current && current != end)
	{
		if (current->type == TOKEN_WORD)
		{
			args[i] = ft_strdup(current->value);
			if (!args[i])
			{
				free_string_array(args, i);
				return (NULL);
			}
			i++;
		}
		current = current->next;
	}
	args[i] = NULL;
	return (args);
}

static int	handle_redirection(t_cmd *cmd, t_token *token)
{
	if (token->type == TOKEN_REDIR_INPUT || token->type == TOKEN_HERE_DOC)
	{
		if (cmd->input_file)
			free(cmd->input_file);
		cmd->input_file = ft_strdup(token->next->value);
		if (!cmd->input_file)
			return (0);
		if (token->type == TOKEN_HERE_DOC)
			return (handle_heredoc(cmd, cmd->input_file, NULL));
	}
	else if (token->type == TOKEN_REDIR_OUTPUT
		|| token->type == TOKEN_REDIR_APPEND)
	{
		if (cmd->output_file)
			free(cmd->output_file);
		cmd->output_file = ft_strdup(token->next->value);
		if (!cmd->output_file)
			return (0);
		cmd->append_output = (token->type == TOKEN_REDIR_APPEND);
	}
	return (1);
}

static t_cmd	*process_command(t_token **current)
{
	t_cmd	*cmd;
	t_token	*start;
	t_token	*token;
	int		arg_count;

	cmd = init_cmd();
	if (!cmd)
		return (NULL);
	start = *current;
	token = start;
	arg_count = count_args(start, NULL);
	while (token && token->type != TOKEN_PIPE)
	{
		if (token->type != TOKEN_WORD)
		{
			if (!handle_redirection(cmd, token))
			{
				free_cmd_list(cmd);
				return (NULL);
			}
			token = token->next; // Skip the redirection target
		}
		token = token->next;
	}
	cmd->args = create_args_array(start, token, arg_count);
	if (!cmd->args)
	{
		free_cmd_list(cmd);
		return (NULL);
	}
	*current = token; // Update current to point to the pipe or NULL
	return (cmd);
}

t_cmd	*group_tokens_into_commands(t_token *token_list, t_env *env)
{
	t_cmd	*first_cmd;
	t_cmd	*current_cmd;
	t_token	*current_token;

	(void)env; // We might need this later for variable expansion
	if (!token_list)
		return (NULL);
	current_token = token_list;
	first_cmd = process_command(&current_token);
	if (!first_cmd)
		return (NULL);
	current_cmd = first_cmd;
	while (current_token && current_token->type == TOKEN_PIPE)
	{
		current_token = current_token->next; // Skip the pipe token
		if (!current_token)
		{
			free_cmd_list(first_cmd);
			return (NULL);
		}
		current_cmd->next = process_command(&current_token);
		if (!current_cmd->next)
		{
			free_cmd_list(first_cmd);
			return (NULL);
		}
		current_cmd = current_cmd->next;
	}
	return (first_cmd);
}
