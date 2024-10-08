/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 20:00:59 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/08 19:10:40 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/execution.h"

static char	*expand_tilde(t_env *env, const char *path)
{
	char	*home;
	char	*expanded_path;

	if (path[0] != '~')
		return (ft_strdup(path));
	home = get_env_value(env, "HOME");
	if (!home)
		return (ft_strdup(path));
	if (path[1] == '\0')
		return (ft_strdup(home));
	expanded_path = ft_strjoin(home, path + 1);
	return (expanded_path);
}

static char	*normalize_path(const char *path)
{
	char	**components;
	char	**normalized;
	int		i;
	int		j;
	char	*result;

	components = ft_split(path, '/');
	normalized = ft_calloc(ft_strlen(path) + 1, sizeof(char *));
	i = 0;
	j = 0;
	while (components[i])
	{
		if (ft_strcmp(components[i], ".") == 0)
			;
		else if (ft_strcmp(components[i], "..") == 0 && j > 0)
			j--;
		else if (ft_strcmp(components[i], "..") != 0)
			normalized[j++] = ft_strdup(components[i]);
		i++;
	}
	result = ft_strjoin("/", ft_strjoin_array(normalized, "/"));
	ft_free_array(components);
	ft_free_array(normalized);
	return (result);
}

static void	change_directory(t_env *env, char *path)
{
	char	*old_pwd;
	char	*new_pwd;

	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
		return (ft_putendl_fd("cd: Error getting current directory", 2));
	if (chdir(path) != 0)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putendl_fd(": No such file or directory", 2);
		free(old_pwd);
		return ;
	}
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		ft_putendl_fd("cd: Error getting new directory", 2);
		free(old_pwd);
		return ;
	}
	update_pwd(env, old_pwd, new_pwd);
	free(old_pwd);
	free(new_pwd);
}

void	builtin_cd(t_env *env, char **args)
{
	char	*path;
	char	*expanded_path;
	char	*normalized_path;

	if (!args[1] || ft_strcmp(args[1], "~") == 0)
		path = get_env_value(env, "HOME");
	else if (ft_strcmp(args[1], "-") == 0)
		path = get_env_value(env, "OLDPWD");
	else
		path = args[1];
	if (!path)
		return (ft_putendl_fd("cd: HOME not set", 2));
	expanded_path = expand_tilde(env, path);
	normalized_path = normalize_path(expanded_path);
	change_directory(env, normalized_path);
	free(expanded_path);
	free(normalized_path);
}
