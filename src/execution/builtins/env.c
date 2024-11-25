/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 19:58:03 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/25 04:24:52 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/execution.h"

static int	write_env_var(const char *key, const char *value)
{
	if (write(STDOUT_FILENO, key, ft_strlen(key)) == -1)
		return (1);
	if (write(STDOUT_FILENO, "=", 1) == -1)
		return (1);
	if (write(STDOUT_FILENO, value, ft_strlen(value)) == -1)
		return (1);
	if (write(STDOUT_FILENO, "\n", 1) == -1)
		return (1);
	return (0);
}

int	builtin_env(t_env *env)
{
	t_env	*current;

	if (!env)
	{
		ft_putendl_fd("minishell: env: environment not available",
			STDERR_FILENO);
		return (1);
	}
	current = env;
	while (current)
	{
		if (current->value && current->key)
		{
			if (write_env_var(current->key, current->value))
			{
				ft_putendl_fd("minishell: env: write error", STDERR_FILENO);
				return (1);
			}
		}
		current = current->next;
	}
	return (0);
}
