/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 18:30:32 by jcohen            #+#    #+#             */
/*   Updated: 2024/11/13 16:21:19 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"
#include "../../includes/token_utils.h"

t_token	*create_token(char *value, t_token_type type, t_quote_type quote_type)
{
	t_token	*token;

	if (!value)
		return (NULL);
	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->type = type;
	token->quote_type = quote_type;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **head, t_token *new_token)
{
	t_token	*current;

	if (!head || !new_token)
		return ;
	if (!*head)
	{
		*head = new_token;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

t_token_type	determine_token_type(char *value)
{
	if (ft_strcmp(value, "|") == 0)
		return (TOKEN_PIPE);
	else if (ft_strcmp(value, "<") == 0)
		return (TOKEN_REDIR_INPUT);
	else if (ft_strcmp(value, ">") == 0)
		return (TOKEN_REDIR_OUTPUT);
	else if (ft_strcmp(value, ">>") == 0)
		return (TOKEN_REDIR_APPEND);
	else if (ft_strcmp(value, "<<") == 0)
		return (TOKEN_HERE_DOC);
	else if (ft_strchr(value, ';'))
	{
		ft_putendl_fd(ERR_UNEXPECTED_SEMICOL, 2);
		return (TOKEN_ERROR);
	}
	else
		return (TOKEN_WORD);
}

int	handle_token(char *input, int i, t_token **head, t_env *env)
{
	if (input[i] == '\'')
		return (token_handle_single_quotes(input, i, head));
	else if (input[i] == '"')
		return (token_handle_double_quotes(input, i, head, env));
	else if (ft_isspace(input[i]))
		return (token_handle_space(input, i));
	else if (ft_strchr("|><", input[i]))
		return (token_handle_redirection(input, i, head));
	else if (input[i] == '$')
		return (token_handle_variable(input, i, head, env));
	else
		return (token_handle_word(input, i, head, env));
}

static int handle_operator(char *input, int i, t_token **head)
{
    char *value;
    int len = 1;
    
    // Check for double operators (>> or <<)
    if ((input[i] == '>' || input[i] == '<') && input[i] == input[i + 1])
        len = 2;
        
    value = ft_substr(input, i, len);
    if (!value)
        return (-1);
        
    t_token_type type = determine_token_type(value);
    if (type == TOKEN_ERROR)
    {
        free(value);
        return (-1);
    }
    
    t_token *token = create_token(value, type, QUOTE_NONE);
    free(value);
    
    if (!token)
        return (-1);
        
    add_token(head, token);
    return (i + len - 1);
}

static int handle_word(char *input, int start, t_token **head, t_env *env)
{
    int end = start;
    
    // Find end of word
    while (input[end] && !is_token_delimiter(input[end]))
        end++;
        
    char *word = ft_substr(input, start, end - start);
    if (!word)
        return (-1);
        
    // Handle empty words
    if (!*word)
    {
        free(word);
        return (end - 1);
    }
    
    // Handle variable expansion
    char *expanded = expand_variables_in_str(word, env, QUOTE_NONE);
    free(word);
    
    if (!expanded)
        return (-1);
        
    // Create token if expanded word is not empty
    if (*expanded)
    {
        t_token *token = create_token(expanded, TOKEN_WORD, QUOTE_NONE);
        free(expanded);
        
        if (!token)
            return (-1);
            
        add_token(head, token);
    }
    else
        free(expanded);
        
    return (end - 1);
}

t_token *tokenizer(char *input, t_env *env)
{
    t_token *head = NULL;
    t_token_state state;
    
    if (!input)
        return (NULL);
        
    init_token_state(&state);
    
    while (input[state.i])
    {
        // Skip whitespace
        if (ft_isspace(input[state.i]))
        {
            state.i++;
            continue;
        }
        
        // Handle quotes
        if (input[state.i] == '"' || input[state.i] == '\'')
        {
            if (handle_quotes(input, &state, &head, env) < 0)
            {
                free_tokens(head);
                return (NULL);
            }
        }
        // Handle operators
        else if (ft_strchr("|<>", input[state.i]))
        {
            int new_i = handle_operator(input, state.i, &head);
            if (new_i < 0)
            {
                free_tokens(head);
                return (NULL);
            }
            state.i = new_i;
        }
        // Handle words and variables
        else
        {
            int new_i = handle_word(input, state.i, &head, env);
            if (new_i < 0)
            {
                free_tokens(head);
                return (NULL);
            }
            state.i = new_i;
        }
        
        state.i++;
    }
    
    // Debug print tokens
    #ifdef DEBUG
    debug_print_token_list(head);
    #endif
    
    return (head);
}