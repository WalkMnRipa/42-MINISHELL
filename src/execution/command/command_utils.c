/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 15:29:51 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/18 15:30:35 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/execution.h"

char	*ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
	char	*temp;
	char	*result;

	temp = ft_strjoin(s1, s2);
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, s3);
	free(temp);
	return (result);
}

int	is_direct_executable(const char *command)
{
	if (command[0] == '/' || command[0] == '.')
	{
		if (access(command, X_OK) == 0)
			return (1);
	}
	return (0);
}

char	*try_path(const char *dir, const char *command)
{
	char	*full_path;

	full_path = ft_strjoin3(dir, "/", command);
	if (!full_path)
		return (NULL);
	if (access(full_path, X_OK) == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}

void	update_exit_status(t_cmd *cmd, int status)
{
	if (WIFEXITED(status))
		cmd->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		cmd->exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
	}
}
