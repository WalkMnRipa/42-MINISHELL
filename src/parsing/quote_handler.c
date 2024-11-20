/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:58:27 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/20 12:29:20 by ggaribot         ###   ########.fr       */
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
        if (result[i] == '\'' || result[i] == '"')
        {
            quote_state = get_quote_state(result[i], quote_state);
            i++;
            continue;
        }
        
        if (result[i] == '$' && quote_state != STATE_SINGLE_QUOTE)
        {
            result = expand_variables_in_str(result, env, quote_state);
            if (!result)
                return (NULL);
            // Skip to next character after handling $
            i++;
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