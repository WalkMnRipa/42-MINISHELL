/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:20:46 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/25 18:11:14 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_strjoin_array(char **array, char *delimiter)
{
	char	*result;
	char	*temp;
	int		i;

	if (!array || !array[0])
		return (ft_strdup(""));
	result = ft_strdup(array[0]);
	i = 1;
	while (array[i])
	{
		temp = result;
		result = ft_strjoin(result, delimiter);
		free(temp);
		temp = result;
		result = ft_strjoin(result, array[i]);
		free(temp);
		i++;
	}
	return (result);
}

void	ft_free_array(char **array)
{
	int	i;

	i = 0;
	if (array)
	{
		while (array[i])
		{
			free(array[i]);
			i++;
		}
		free(array);
	}
}

char	*ft_strtok(char *str, const char *delim)
{
	static char	*last;
	char		*token;

	if (str)
		last = str;
	if (!last || !*last)
		return (NULL);
	while (*last && ft_strchr(delim, *last))
		last++;
	if (!*last)
		return (NULL);
	token = last;
	while (*last && !ft_strchr(delim, *last))
		last++;
	if (*last)
	{
		*last = '\0';
		last++;
	}
	return (token);
}

static int	fill_env_array(char **env_array, t_env *env)
{
	t_env	*current;
	int		i;

	i = 0;
	current = env;
	while (current)
	{
		env_array[i] = ft_strjoin3(current->key, "=", current->value);
		if (!env_array[i])
			return (free_string_array(env_array, i), 0);
		current = current->next;
		i++;
	}
	env_array[i] = NULL;
	return (1);
}

char	**env_to_array(t_env *env)
{
	t_env	*current;
	char	**env_array;
	int		count;

	count = 0;
	current = env;
	while (current)
	{
		count++;
		current = current->next;
	}
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	if (!fill_env_array(env_array, env))
		return (NULL);
	return (env_array);
}
