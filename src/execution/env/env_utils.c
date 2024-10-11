/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 20:13:29 by ggaribot          #+#    #+#             */
<<<<<<< HEAD:src/execution/env_utils.c
/*   Updated: 2024/10/08 19:27:10 by jcohen           ###   ########.fr       */
=======
/*   Updated: 2024/10/11 16:24:05 by ggaribot         ###   ########.fr       */
>>>>>>> ec5ff35 (ordonner, aucun changement dans code a part makefile):src/execution/env/env_utils.c
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/execution.h"

t_env	*create_env_node(char *envp)
{
	t_env	*new_node;
	char	*equals_sign;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	equals_sign = ft_strchr(envp, '=');
	if (equals_sign)
	{
		new_node->key = ft_substr(envp, 0, equals_sign - envp);
		new_node->value = ft_strdup(equals_sign + 1);
	}
	else
	{
		new_node->key = ft_strdup(envp);
		new_node->value = ft_strdup("");
	}
	new_node->next = NULL;
	return (new_node);
}

t_env	*init_env(char **envp)
{
	t_env	*env;
	t_env	*new_node;

	env = NULL;
	while (*envp)
	{
		new_node = create_env_node(*envp);
		if (!new_node)
		{
			free_env(env);
			return (NULL);
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

<<<<<<< HEAD:src/execution/env_utils.c
// ft_strjoin_free 

=======
int	create_env_var(t_env **env, const char *name, const char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (-1);
	new_node->key = ft_strdup(name);
	new_node->value = ft_strdup(value);
	new_node->next = *env;
	*env = new_node;
	if (!new_node->key || !new_node->value)
	{
		free(new_node->key);
		free(new_node->value);
		free(new_node);
		return (-1);
	}
	return (0);
}
>>>>>>> ec5ff35 (ordonner, aucun changement dans code a part makefile):src/execution/env/env_utils.c
