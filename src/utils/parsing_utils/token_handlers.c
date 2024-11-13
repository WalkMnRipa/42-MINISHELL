/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:00:39 by jcohen            #+#    #+#             */
/*   Updated: 2024/11/13 12:45:18 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parsing.h"

static int is_special_char(char c)
{
    return (c == '|' || c == '<' || c == '>' || c == ';' || 
            c == '(' || c == ')' || c == ' ' || c == '\t' ||
            c == '\n' || c == '\0');
}

int token_handle_single_quotes(char *input, int i, t_token **head)
{
    int     end;
    char    *content;
    t_token *new_token;

    if (!input[i + 1])
        return (-1);
    end = get_quote_end(input, i + 1, '\'');
    if (end <= i + 1)
        return (-1);
    content = ft_substr(input, i + 1, end - (i + 1));
    if (!content)
        return (-1);
    new_token = create_token(content, TOKEN_WORD, QUOTE_SINGLE);
    free(content);
    if (!new_token)
        return (-1);
    add_token(head, new_token);
    return (end);
}

int token_handle_double_quotes(char *input, int i, t_token **head, t_env *env)
{
    int     end;
    char    *content;
    char    *expanded;
    t_token *new_token;

    if (!input[i + 1])
        return (-1);
    end = get_quote_end(input, i + 1, '"');
    if (end <= i + 1)
        return (-1);
    content = ft_substr(input, i + 1, end - (i + 1));
    if (!content)
        return (-1);
    expanded = expand_variables_in_str(content, env, QUOTE_DOUBLE);
    free(content);
    if (!expanded)
        return (-1);
    new_token = create_token(expanded, TOKEN_WORD, QUOTE_DOUBLE);
    free(expanded);
    if (!new_token)
        return (-1);
    add_token(head, new_token);
    return (end);
}

int token_handle_word(char *input, int i, t_token **head, t_env *env)
{
    int     end;
    char    *value;
    char    *expanded;
    t_token *new_token;

    end = i;
    // Stop at any special character
    while (input[end] && !is_special_char(input[end]))
        end++;

    // If we find a special character, back up one position
    if (is_special_char(input[end]))
        end = (end > i) ? end : end + 1;

    value = ft_substr(input, i, end - i);
    if (!value)
        return (-1);

    // Check for semicolon in the word
    if (ft_strchr(value, ';'))
    {
        ft_putendl_fd(ERR_UNEXPECTED_SEMICOL, 2);
        free(value);
        return (-1);
    }

    expanded = expand_variables_in_str(value, env, QUOTE_NONE);
    free(value);
    if (!expanded)
        return (-1);

    if (*expanded)  // Only create token if content is not empty
    {
        new_token = create_token(expanded, TOKEN_WORD, QUOTE_NONE);
        if (!new_token)
        {
            free(expanded);
            return (-1);
        }
        add_token(head, new_token);
    }
    free(expanded);
    return (end - 1);
}

int token_handle_redirection(char *input, int i, t_token **head)
{
    char    *value;
    t_token *new_token;
    int     len;

    len = 1;
    if (input[i + 1] && (input[i] == input[i + 1]) && 
        (input[i] == '>' || input[i] == '<'))
        len = 2;
    value = ft_substr(input, i, len);
    if (!value)
        return (-1);
    new_token = create_token(value, determine_token_type(value), QUOTE_NONE);
    free(value);
    if (!new_token)
        return (-1);
    add_token(head, new_token);
    return (i + len - 1);
}
