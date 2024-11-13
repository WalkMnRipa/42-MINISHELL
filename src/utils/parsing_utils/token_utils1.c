/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:18:41 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/13 16:18:59 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/token_utils.h"

void init_token_state(t_token_state *state)
{
    state->in_quotes = 0;
    state->quote_char = '\0';
    state->escape_next = 0;
    state->word_start = 0;
    state->i = 0;
}

int is_token_delimiter(char c)
{
    return (c == ' ' || c == '\t' || c == '|' || c == '<' || c == '>' || 
            c == ';' || c == '\n' || c == '\0');
}

int handle_quotes(char *input, t_token_state *state, t_token **head, t_env *env)
{
    char quote = input[state->i];
    int start = state->i + 1;
    int end = start;
    
    // Find matching quote
    while (input[end] && input[end] != quote)
        end++;
        
    if (!input[end])
        return (-1); // Unclosed quote
        
    // Extract quoted content
    char *content = ft_substr(input, start, end - start);
    if (!content)
        return (-1);
        
    // Handle expansion if needed
    char *final_content;
    if (quote == '"')
        final_content = expand_variables_in_str(content, env, QUOTE_DOUBLE);
    else
        final_content = ft_strdup(content);
        
    free(content);
    if (!final_content)
        return (-1);
        
    // Create token
    t_token *token = create_token(final_content, TOKEN_WORD, 
                                quote == '"' ? QUOTE_DOUBLE : QUOTE_SINGLE);
    free(final_content);
    
    if (!token)
        return (-1);
        
    add_token(head, token);
    state->i = end;
    return (0);
}

void debug_print_token(t_token *token)
{
    const char *type_str[] = {
        "WORD", "PIPE", "REDIR_INPUT", "REDIR_OUTPUT",
        "REDIR_APPEND", "HERE_DOC", "VARIABLE", "ERROR"
    };
    
    const char *quote_str[] = {
        "NONE", "SINGLE", "DOUBLE"
    };
    
    printf("Token: {\n");
    printf("  type: %s\n", type_str[token->type]);
    printf("  value: '%s'\n", token->value);
    printf("  quote_type: %s\n", quote_str[token->quote_type]);
    printf("}\n");
}

void debug_print_token_list(t_token *head)
{
    printf("\n=== Token List ===\n");
    while (head)
    {
        debug_print_token(head);
        head = head->next;
    }
    printf("================\n\n");
}