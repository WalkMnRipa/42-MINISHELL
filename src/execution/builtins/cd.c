/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 20:00:59 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/10 17:25:55 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/execution.h"

static char	*expand_tilde(t_env *env, const char *path)
{
	char	*home;
	char	*result;

	if (!path || path[0] != '~')
		return (ft_strdup(path));
	home = get_env_value(env, "HOME");
	if (!home)
		return (ft_strdup(path));
	if (path[1] == '\0')
		result = ft_strdup(home);
	else
		result = ft_strjoin(home, path + 1);
	return (result);
}

static int	change_directory(t_env *env, const char *path)
{
	char	*old_pwd;
	char	*new_pwd;

	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
		return (0);
	if (chdir(path) != 0)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd((char *)path, 2);
		ft_putendl_fd(": No such file or directory", 2);
		free(old_pwd);
		return (0);
	}
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		ft_putendl_fd("cd: Error getting new directory", 2);
		free(old_pwd);
		return (0);
	}
	update_pwd(env, old_pwd, new_pwd);
	return (1);
}

void	builtin_cd(t_env *env, char **args)
{
	char	*path;
	char	*expanded_path;

	if (!args || !args[1] || ft_strcmp(args[1], "~") == 0)
		path = get_env_value(env, "HOME");
	else if (ft_strcmp(args[1], "-") == 0)
		path = get_env_value(env, "OLDPWD");
	else
		path = args[1];
	if (!path)
	{
		ft_putendl_fd("cd: HOME not set", 2);
		return ;
	}
	expanded_path = expand_tilde(env, path);
	if (!expanded_path)
	{
		ft_putendl_fd("cd: Memory allocation error", 2);
		return ;
	}
	change_directory(env, expanded_path);
	free(expanded_path);
}
