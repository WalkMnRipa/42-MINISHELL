/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 19:57:43 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/25 18:08:47 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	builtin_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		if (errno == EACCES)
			ft_putendl_fd("minishell: pwd: permission denied", STDERR_FILENO);
		else if (errno == ENOMEM)
			ft_putendl_fd("minishell: pwd: memory allocation failed",
				STDERR_FILENO);
		else
			ft_putendl_fd("minishell: pwd: error retrieving current directory",
				STDERR_FILENO);
		return (1);
	}
	ft_putendl_fd(pwd, STDOUT_FILENO);
	free(pwd);
	return (0);
}
