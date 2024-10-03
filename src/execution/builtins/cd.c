/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 20:00:59 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/02 18:29:42 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/execution.h"

void	builtin_cd(t_shell *shell, char **args)
{
	char	*path;
	char	*old_pwd;
	char	*new_pwd;

	if (!args[1] || ft_strncmp(args[1], "~", 2) == 0)
		path = getenv("HOME");
	else if (ft_strncmp(args[1], "-", 2) == 0)
		path = getenv("OLDPWD");
	else
		path = args[1];
	if (!path)
		return (ft_putendl_fd("cd: HOME not set", 2));
	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
		return (ft_putendl_fd("cd: Error getting current directory", 2));
	if (chdir(path) != 0)
		return (ft_putstr_fd("cd: ", 2), ft_putstr_fd(path, 2),
			ft_putendl_fd(": No such file or directory", 2), free(old_pwd));
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
		return (ft_putendl_fd("cd: Error getting new directory", 2),
			free(old_pwd));
	update_pwd(shell, old_pwd, new_pwd);
	free(old_pwd);
	free(new_pwd);
}
