/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:59:44 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/20 12:28:59 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

static char *extract_var_name(char *str, int *i)
{
    int     start;
    int     len;
    char    *name;

    start = *i + 1;
    len = 0;
    if (str[start] == '?')
    {
        *i += 1;
        return (ft_strdup("?"));
    }
    while (str[start + len] && (ft_isalnum(str[start + len]) || 
           str[start + len] == '_'))
        len++;
    if (len == 0)
        return (NULL);
    name = ft_substr(str, start, len);
    *i += len;
    return (name);
}

static char *handle_exit_status(t_env *env)
{
    return (ft_itoa(env->last_exit_status));
}

static char *expand_single_var(char *str, int *i, t_env *env)
{
    char    *var_name;
    char    *var_value;
    char    *before;
    char    *after;
    char    *temp;
    char    *result;

    var_name = extract_var_name(str, i);
    if (!var_name)
        return (str);

    before = ft_substr(str, 0, *i - ft_strlen(var_name));
    after = ft_strdup(str + *i + 1);
    if (!before || !after)
    {
        free(var_name);
        free(before);
        free(after);
        return (NULL);
    }

    if (ft_strcmp(var_name, "?") == 0)
        var_value = handle_exit_status(env);
    else
        var_value = ft_strdup(get_env_value(env, var_name));

    if (!var_value)
        var_value = ft_strdup("");

    temp = ft_strjoin(before, var_value);
    result = ft_strjoin(temp, after);

    free(var_name);
    free(var_value);
    free(before);
    free(after);
    free(temp);
    free(str);

    *i = ft_strlen(before) + ft_strlen(var_value) - 1;
    return (result);
}

char *expand_variables_in_str(char *str, t_env *env, t_quote_state state)
{
    int     i;
    char    *result;

    if (!str)
        return (NULL);

    result = str;
    i = 0;
    while (result[i])
    {
        if (result[i] == '$' && state != STATE_SINGLE_QUOTE)
        {
            // Skip $ if it's followed by a special character or end of string
            if (!result[i + 1] || result[i + 1] == ' ' || 
                result[i + 1] == '"' || result[i + 1] == '\'' || 
                result[i + 1] == '\\' || result[i + 1] == '<' || 
                result[i + 1] == '>' || result[i + 1] == '|' || 
                result[i + 1] == '&')
            {
                i++;
                continue;
            }

            // Only try to expand if next char is alphanumeric or underscore
            if (ft_isalnum(result[i + 1]) || result[i + 1] == '_' || result[i + 1] == '?')
            {
                result = expand_single_var(result, &i, env);
                if (!result)
                    return (NULL);
            }
            else
                i++;
            continue;
        }
        i++;
    }
    return (result);
}