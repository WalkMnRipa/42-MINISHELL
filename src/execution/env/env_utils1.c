/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 16:25:09 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/11 16:26:25 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/execution.h"

int	is_valid_env_name(const char *name)
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

int	is_readonly_var(const char *name)
{
	if (ft_strcmp(name, "PWD") == 0)
		return (1);
	if (ft_strcmp(name, "OLDPWD") == 0)
		return (1);
	return (0);
}

void	remove_env_var(t_env **env, const char *name)
{
	t_env *current;
	t_env *prev;

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