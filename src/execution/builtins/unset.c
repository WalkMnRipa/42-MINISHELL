/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 20:01:42 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/08 16:54:11 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/execution.h"

static int	is_valid_env_name(const char *name)
{
	int	i;

	if (!name || !*name || ft_isdigit(*name))
		return (0);
	i = 0;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	is_readonly_var(const char *name)
{
	const char	*readonly_vars[] = {"PWD", "OLDPWD", NULL};
	int			i;

	i = 0;
	while (readonly_vars[i])
	{
		if (ft_strcmp(name, readonly_vars[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

void	remove_env_var(t_env **env, const char *name)
{
	t_env	*current;
	t_env	*prev;

	current = *env;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, name) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

void	builtin_unset(t_env **env, char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		remove_env_var(env, args[i]);
		i++;
	}
}