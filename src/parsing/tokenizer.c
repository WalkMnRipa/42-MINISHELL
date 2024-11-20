/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:57:57 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/20 12:34:05 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

static char *extract_word(char **input, t_env *env)
{
    char        *start;
    int         len;
    char        *word;
    t_quote_state quote_state;
    
    start = *input;
    len = 0;
    quote_state = STATE_NORMAL;
    
    while ((*input)[len])
    {
        if (is_quote((*input)[len]))
            quote_state = get_quote_state((*input)[len], quote_state);
            
        if (quote_state == STATE_NORMAL && !is_quote((*input)[len]))
        {
            if (is_whitespace((*input)[len]) || 
                (is_operator((*input)[len]) && len == 0))
                break;
        }
        len++;
    }
    
    word = ft_substr(start, 0, len);
    if (!word)
        return (NULL);
    
    *input += len;
    return (handle_quotes(word, env));
}

static t_token_type get_operator_type(char *str)
{
    if (str[0] == '|')
        return (TOKEN_PIPE);
    else if (str[0] == '<')
    {
        if (str[1] == '<')
            return (TOKEN_HEREDOC);
        return (TOKEN_REDIR_IN);
    }
    else if (str[0] == '>')
    {
        if (str[1] == '>')
            return (TOKEN_REDIR_APPEND);
        return (TOKEN_REDIR_OUT);
    }
    return (TOKEN_OPERATOR);
}

static t_token *handle_operator(char **input)
{
    t_token_type    type;
    char            *value;
    int             len;
    
    len = 1;
    if ((*input)[0] == (*input)[1] && ((*input)[0] == '<' || (*input)[0] == '>'))
        len = 2;
    
    value = ft_substr(*input, 0, len);
    if (!value)
        return (NULL);
    
    type = get_operator_type(*input);
    *input += len;
    
    return (create_token(type, value));
}

t_token *get_next_token(char **input, t_env *env)
{
    while (**input && is_whitespace(**input))
        (*input)++;
    
    if (!**input)
        return (create_token(TOKEN_EOF, NULL));
    
    if (is_operator(**input))
        return (handle_operator(input));
    
    return (create_token(TOKEN_WORD, extract_word(input, env)));
}

t_token *tokenizer(char *input, t_env *env)
{
    t_token *head;
    t_token *new_token;
    char    *current;
    
    if (!input || !*input)
        return (NULL);
    
    head = NULL;
    current = input;
    
    while (*current)
    {
        new_token = get_next_token(&current, env);
        if (!new_token)
        {
            free_tokens(head);
            return (NULL);
        }
        
        if (new_token->type == TOKEN_EOF)
        {
            free_tokens(new_token);
            break;
        }
        
        add_token(&head, new_token);
    }
    
    if (!check_syntax_errors(head))
    {
        free_tokens(head);
        return (NULL);
    }
    
    return (head);
}