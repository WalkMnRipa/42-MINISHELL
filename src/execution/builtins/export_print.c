/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:43:11 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/25 18:08:38 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static void	print_env_entry(t_env *entry)
{
	ft_putstr_fd("declare -x ", 1);
	ft_putstr_fd(entry->key, 1);
	if (entry->value && *entry->value)
	{
		ft_putstr_fd("=\"", 1);
		ft_putstr_fd(entry->value, 1);
		ft_putchar_fd('\"', 1);
	}
	ft_putchar_fd('\n', 1);
}

static int	env_list_size(t_env *env)
{
	int		size;
	t_env	*current;

	size = 0;
	current = env;
	while (current)
	{
		size++;
		current = current->next;
	}
	return (size);
}

static void	sort_env_array(t_env **arr, int size)
{
	int		i;
	int		j;
	t_env	*temp;

	i = 0;
	while (i < size - 1)
	{
		j = 0;
		while (j < size - i - 1)
		{
			if (ft_strcmp(arr[j]->key, arr[j + 1]->key) > 0)
			{
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

static t_env	**create_env_array(t_env *env, int size)
{
	t_env	**arr;
	t_env	*current;
	int		i;

	arr = malloc(sizeof(t_env *) * size);
	if (!arr)
		return (NULL);
	current = env;
	i = 0;
	while (current)
	{
		arr[i] = current;
		current = current->next;
		i++;
	}
	return (arr);
}

int	print_sorted_env(t_env *env)
{
	t_env	**env_arr;
	int		size;
	int		i;

	if (!env)
		return (0);
	size = env_list_size(env);
	env_arr = create_env_array(env, size);
	if (!env_arr)
		return (1);
	sort_env_array(env_arr, size);
	i = 0;
	while (i < size)
	{
		print_env_entry(env_arr[i]);
		i++;
	}
	free(env_arr);
	return (0);
}
