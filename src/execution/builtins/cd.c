/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 20:00:59 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/26 15:01:06 by ggaribot         ###   ########.fr       */
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

static int	handle_directory_error(const char *path)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd((char *)path, 2);
	ft_putendl_fd(": No such file or directory", 2);
	return (1);
}

static int	change_directory(t_env *env, const char *path)
{
	char	*old_pwd;
	char	*new_pwd;

	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
		return (1);
	if (chdir(path) != 0)
	{
		free(old_pwd);
		return (handle_directory_error(path));
	}
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		ft_putendl_fd("cd: Error getting new directory", 2);
		free(old_pwd);
		return (1);
	}
	update_pwd(env, old_pwd, new_pwd);
	return (0);
}

static char	*get_cd_path(t_env *env, char **args, int *exit_status)
{
	char	*path;

	if (!args[1] || ft_strcmp(args[1], "~") == 0)
	{
		path = get_env_value(env, "HOME");
		if (!path)
		{
			ft_putendl_fd("cd: HOME not set", 2);
			*exit_status = 1;
			return (NULL);
		}
		return (ft_strdup(path));
	}
	if (ft_strcmp(args[1], "-") == 0)
	{
		path = get_env_value(env, "OLDPWD");
		if (!path)
		{
			ft_putendl_fd("cd: OLDPWD not set", 2);
			*exit_status = 1;
			return (NULL);
		}
		return (ft_strdup(path));
	}
	return (expand_tilde(env, args[1]));
}

void	builtin_cd(t_env *env, char **args)
{
	char	*path;

	if (!args)
		return ;
	if (args[1] && args[2])
	{
		ft_putendl_fd("minishell: cd: too many arguments", 2);
		env->last_exit_status = 1;
		return ;
	}
	path = get_cd_path(env, args, &env->last_exit_status);
	if (!path)
		return ;
	env->last_exit_status = change_directory(env, path);
	free(path);
}
