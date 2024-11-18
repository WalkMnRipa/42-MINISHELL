/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 20:13:29 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/18 15:33:22 by ggaribot         ###   ########.fr       */
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
	if (!new_node->key || !new_node->value)
	{
		free(new_node->key);
		free(new_node->value);
		free(new_node);
		return (NULL);
	}
	return (new_node->next = NULL, new_node);
}

t_env	*init_env(char **envp)
{
	t_env	*env;
	t_env	*new_node;
	t_env	*last_node;

	env = NULL;
	last_node = NULL;
	while (*envp)
	{
		new_node = create_env_node(*envp);
		if (!new_node)
		{
			free_env(env);
			return (NULL);
		}
		if (!env)
			env = new_node;
		else
			last_node->next = new_node;
		last_node = new_node;
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
