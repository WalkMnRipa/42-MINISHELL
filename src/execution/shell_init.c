/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 18:27:54 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/01 19:56:29 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

static char	*find_path(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

static char	**get_cmd_paths(char **envp)
{
	char	*path;

	path = find_path(envp);
	if (!path)
		return (NULL);
	return (ft_split(path, ':'));
}

void	initialize_shell(t_shell *shell, char **envp)
{
	shell->env = ft_split(envp[0], '\0');
	if (!shell->env)
		error_exit(shell, "Error: Failed to initialize shell environment");
	shell->cmd_paths = get_cmd_paths(envp);
	if (!shell->cmd_paths)
		error_exit(shell, "Error: Failed to get command paths");
}
