/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_pwd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 17:54:26 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/10 15:12:14 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/execution.h"

static int	create_env_var(t_env **env, const char *name, const char *value)
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

int	custom_setenv(t_env **env, const char *name, const char *value)
{
	t_env	*current;

	if (!env || !*env || !name || !value)
		return (-1);
	current = *env;
	while (current)
	{
		if (ft_strcmp(current->key, name) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			if (!current->value)
				return (-1);
			return (0);
		}
		current = current->next;
	}
	return (create_env_var(env, name, value));
}

void	update_pwd(t_env *env, char *old_pwd, char *new_pwd)
{
	if (!env || !old_pwd)
	{
		free(old_pwd);
		free(new_pwd);
		return ;
	}
	if (new_pwd)
	{
		if (custom_setenv(&env, "OLDPWD", old_pwd) == -1)
			ft_putendl_fd("minishell: failed to update OLDPWD", 2);
		if (custom_setenv(&env, "PWD", new_pwd) == -1)
			ft_putendl_fd("minishell: failed to update PWD", 2);
	}
	else
	{
		ft_putendl_fd("minishell: failed to get new working directory", 2);
	}
	free(old_pwd);
	free(new_pwd);
}
