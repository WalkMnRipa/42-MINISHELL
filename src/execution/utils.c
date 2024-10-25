/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:20:46 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/25 16:46:30 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

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
