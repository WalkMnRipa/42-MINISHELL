/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 20:00:59 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/25 04:24:37 by ggaribot         ###   ########.fr       */
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
	if (!result)
		return (NULL);
	return (result);
}

static int	handle_directory_error(const char *path)
{
	struct stat	path_stat;

	if (access(path, F_OK) == -1)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd((char *)path, STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		return (1);
	}
	if (access(path, R_OK | X_OK) == -1)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd((char *)path, STDERR_FILENO);
		ft_putendl_fd(": Permission denied", STDERR_FILENO);
		return (1);
	}
	if (stat(path, &path_stat) == 0 && !S_ISDIR(path_stat.st_mode))
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd((char *)path, STDERR_FILENO);
		ft_putendl_fd(": Not a directory", STDERR_FILENO);
		return (1);
	}
	return (0);
}

static int	change_directory(t_env *env, const char *path)
{
	char	*old_pwd;
	char	*new_pwd;

	if (handle_directory_error(path))
		return (1);
	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
	{
		ft_putendl_fd("minishell: cd: error retrieving current directory",
			STDERR_FILENO);
		return (1);
	}
	if (chdir(path) != 0)
		return (free(old_pwd), 1);
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		ft_putendl_fd("minishell: cd: error retrieving new directory",
			STDERR_FILENO);
		free(old_pwd);
		return (1);
	}
	update_pwd(env, old_pwd, new_pwd);
	return (0);
}

static char	*get_cd_path(t_env *env, char **args)
{
	char	*path;

	if (!args[1] || ft_strcmp(args[1], "~") == 0)
	{
		path = get_env_value(env, "HOME");
		if (!path)
		{
			ft_putendl_fd("minishell: cd: HOME not set", STDERR_FILENO);
			return (NULL);
		}
		return (ft_strdup(path));
	}
	if (ft_strcmp(args[1], "-") == 0)
	{
		path = get_env_value(env, "OLDPWD");
		if (!path)
		{
			ft_putendl_fd("minishell: cd: OLDPWD not set", STDERR_FILENO);
			return (NULL);
		}
		return (ft_strdup(path));
	}
	return (expand_tilde(env, args[1]));
}

int	builtin_cd(t_cmd *cmd, t_env *env, char **args)
{
	char	*path;

	if (!args)
		return (1);
	if (args[1] && args[2])
	{
		ft_putendl_fd("minishell: cd: too many arguments", STDERR_FILENO);
		return (1);
	}
	path = get_cd_path(env, args);
	if (!path)
		return (1);
	cmd->exit_status = change_directory(env, path);
	free(path);
	return (cmd->exit_status);
}
