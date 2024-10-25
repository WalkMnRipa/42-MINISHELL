/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 20:00:59 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/25 18:17:25 by jcohen           ###   ########.fr       */
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
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd((char *)path, 2);
		perror(" ");
		free(old_pwd);
		return (0);
	}
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		ft_putendl_fd("minishell: cd: error getting new directory", 2);
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
	int		arg_count;
	int		cd_result;

	arg_count = 0;
	while (args[arg_count])
		arg_count++;
	if (arg_count > 2)
	{
		ft_putendl_fd("minishell: cd: too many arguments", 2);
		env->last_exit_status = 1;
		return ;
	}
	if (!args || !args[1] || ft_strcmp(args[1], "~") == 0)
		path = get_env_value(env, "HOME");
	else if (ft_strcmp(args[1], "-") == 0)
		path = get_env_value(env, "OLDPWD");
	else
		path = args[1];
	if (!path)
	{
		ft_putendl_fd("minishell: cd: HOME not set", 2);
		env->last_exit_status = 1;
		return ;
	}
	expanded_path = expand_tilde(env, path);
	if (!expanded_path)
	{
		ft_putendl_fd("minishell: cd: memory allocation error", 2);
		env->last_exit_status = 1;
		return ;
	}
	cd_result = change_directory(env, expanded_path);
	free(expanded_path);
	env->last_exit_status = cd_result ? 0 : 1;
}
