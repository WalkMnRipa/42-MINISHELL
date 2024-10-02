/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 19:58:54 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/01 19:59:02 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

void	free_string_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

char	*get_cmd(char **paths, char *cmd)
{
	char	*tmp;
	char	*command;

	if (!cmd)
		return (NULL);
	if (cmd[0] == '/' || ft_strncmp(cmd, "./", 2) == 0)
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	while (*paths)
	{
		tmp = ft_strjoin(*paths, "/");
		command = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(command, X_OK) == 0)
			return (command);
		free(command);
		paths++;
	}
	return (NULL);
}

char	*ft_strjoin_shell(char const *s1, char const *s2)
{
	size_t	len1;
	size_t	len2;
	char	*new_str;

	if (!s1)
		len1 = 0;
	else
		len1 = ft_strlen(s1);
	if (!s2)
		len2 = 0;
	else
		len2 = ft_strlen(s2);
	new_str = malloc(sizeof(char) * (len1 + len2 + 1));
	if (new_str == NULL)
		return (NULL);
	if (s1)
		ft_strlcpy(new_str, s1, len1 + 1);
	if (s2)
		ft_strlcpy(new_str + len1, s2, len2 + 1);
	new_str[len1 + len2] = '\0';
	return (new_str);
}
