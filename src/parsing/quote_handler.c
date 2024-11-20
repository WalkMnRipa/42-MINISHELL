/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:58:27 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/20 11:15:56 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

t_quote_state get_quote_state(char c, t_quote_state current)
{
    if (current == STATE_NORMAL)
    {
        if (c == '\'')
            return (STATE_SINGLE_QUOTE);
        if (c == '"')
            return (STATE_DOUBLE_QUOTE);
    }
    else if (current == STATE_SINGLE_QUOTE && c == '\'')
        return (STATE_NORMAL);
    else if (current == STATE_DOUBLE_QUOTE && c == '"')
        return (STATE_NORMAL);
    return (current);
}

static int should_preserve_quote(char quote_char, t_quote_state outer_state)
{
    // Preserve quotes if they're inside the other type of quotes
    if (outer_state == STATE_SINGLE_QUOTE && quote_char == '"')
        return (1);
    if (outer_state == STATE_DOUBLE_QUOTE && quote_char == '\'')
        return (1);
    return (0);
}

static char *process_backslash(char *str, int *i, t_quote_state quote_state)
{
    if (quote_state == STATE_DOUBLE_QUOTE)
    {
        if (str[*i + 1] == '"' || str[*i + 1] == '\\' || str[*i + 1] == '$')
            ft_memmove(&str[*i], &str[*i + 1], ft_strlen(&str[*i]));
    }
    else if (quote_state == STATE_NORMAL)
        ft_memmove(&str[*i], &str[*i + 1], ft_strlen(&str[*i]));
    return (str);
}

char *handle_quotes(char *str, t_env *env)
{
    int             i;
    t_quote_state   quote_state;
    char            *result;
    
    if (!str)
        return (NULL);
    
    result = ft_strdup(str);
    if (!result)
        return (NULL);
    
    i = 0;
    quote_state = STATE_NORMAL;
    while (result[i])
    {
        if (result[i] == '\\')
            result = process_backslash(result, &i, quote_state);
        else if (result[i] == '\'' || result[i] == '"')
        {
            if (should_preserve_quote(result[i], quote_state))
            {
                i++; // Keep the quote and move on
                continue;
            }
            quote_state = get_quote_state(result[i], quote_state);
            ft_memmove(&result[i], &result[i + 1], ft_strlen(&result[i]));
            continue;
        }
        else if (result[i] == '$' && quote_state != STATE_SINGLE_QUOTE)
        {
            result = expand_variables_in_str(result, env, quote_state);
            if (!result)
                return (NULL);
            continue;
        }
        i++;
    }
    
    free(str);
    return (result);
}

int is_quote(char c)
{
    return (c == '\'' || c == '"');
}

int is_quote_closed(char *str)
{
    t_quote_state   state;
    int             i;
    
    state = STATE_NORMAL;
    i = 0;
    while (str[i])
    {
        if (is_quote(str[i]))
            state = get_quote_state(str[i], state);
        i++;
    }
    
    return (state == STATE_NORMAL);
}