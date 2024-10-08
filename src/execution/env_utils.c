/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 20:13:29 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/07 17:03:46 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

t_env	*init_env(char **envp)
{
	t_env	*env;
	t_env	*new_node;
	char	*equals_sign;

	env = NULL;
	while (*envp)
	{
		new_node = malloc(sizeof(t_env));
		if (!new_node)
			return (NULL);
		equals_sign = ft_strchr(*envp, '=');
		if (equals_sign)
		{
			new_node->key = ft_substr(*envp, 0, equals_sign - *envp);
			new_node->value = ft_strdup(equals_sign + 1);
		}
		else
		{
			new_node->key = ft_strdup(*envp);
			new_node->value = ft_strdup("");
		}
		new_node->next = env;
		env = new_node;
		envp++;
	}
	return (env);
}

char	*get_env_value(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}