/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_processor.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 00:48:44 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/19 00:48:46 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

void	init_var_processor(t_var_processor *vp, char *input, t_env *env)
{
	vp->input = input;
	vp->result = NULL;
	vp->env = env;
	vp->i = 0;
	vp->in_quotes = 0;
}

static char	*handle_special_var(t_env *env, char special_char)
{
	if (special_char == '?')
		return (ft_itoa(env->last_exit_status));
	else if (special_char == '$')
		return (ft_itoa(getpid()));
	return (ft_strdup(""));
}

static char	*get_env_var_value(t_var_processor *vp, int start)
{
	char	*name;
	char	*value;
	int		end;

	end = start;
	while (vp->input[end] && (ft_isalnum(vp->input[end])
			|| vp->input[end] == '_'))
		end++;
	name = ft_substr(vp->input, start, end - start);
	if (!name)
		return (NULL);
	value = get_env_value(vp->env, name);
	free(name);
	if (!value)
		return (ft_strdup(""));
	vp->i = end - 1;
	return (ft_strdup(value));
}

static char	*append_str(char *s1, char *s2)
{
	char	*result;

	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (s1);
	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}

char	*process_variables(char *input, t_env *env, int in_quotes)
{
	t_var_processor	vp;
	char			*result;
	char			*tmp;

	if (!input)
		return (NULL);
	init_var_processor(&vp, input, env);
	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (input[vp.i])
	{
		if (input[vp.i] == '$' && (!in_quotes || vp.in_quotes == 2))
		{
			vp.i++;
			if (!input[vp.i] || ft_isspace(input[vp.i]))
				tmp = ft_strdup("$");
			else if (input[vp.i] == '?' || input[vp.i] == '$')
				tmp = handle_special_var(env, input[vp.i]);
			else
				tmp = get_env_var_value(&vp, vp.i);
			result = append_str(result, tmp);
			free(tmp);
			if (!result)
				return (NULL);
		}
		else
		{
			tmp = ft_substr(input, vp.i, 1);
			result = append_str(result, tmp);
			free(tmp);
			if (!result)
				return (NULL);
		}
		vp.i++;
	}
	return (result);
}
