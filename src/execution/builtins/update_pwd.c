/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_pwd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 17:54:26 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/02 17:59:09 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/execution.h"

static int	count_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	return (i);
}

static char	*create_env_var(const char *name, const char *value)
{
	char	*new_var;
	char	*temp;

	temp = ft_strjoin(name, "=");
	if (!temp)
		return (NULL);
	new_var = ft_strjoin(temp, value);
	free(temp);
	return (new_var);
}

static int	update_existing_env(char **env, const char *name, const char *value)
{
	int		i;
	size_t	name_len;

	i = 0;
	name_len = ft_strlen(name);
	while (env[i])
	{
		if (ft_strncmp(env[i], name, name_len) == 0 && env[i][name_len] == '=')
		{
			free(env[i]);
			env[i] = create_env_var(name, value);
			if (env[i])
				return (0);
			return (-1);
		}
		i++;
	}
	return (1);
}

static int	custom_setenv(t_shell *shell, const char *name, const char *value)
{
	char	**new_env;
	char	*new_var;
	int		env_count;
	int		update_result;

	update_result = update_existing_env(shell->env, name, value);
	if (update_result <= 0)
		return (update_result);
	env_count = count_env(shell->env);
	new_env = ft_calloc(env_count + 2, sizeof(char *));
	if (!new_env)
		return (-1);
	ft_memcpy(new_env, shell->env, env_count * sizeof(char *));
	new_var = create_env_var(name, value);
	if (!new_var)
	{
		free(new_env);
		return (-1);
	}
	new_env[env_count] = new_var;
	free(shell->env);
	shell->env = new_env;
	return (0);
}

void	update_pwd(t_shell *shell, char *old_pwd, char *new_pwd)
{
	if (new_pwd)
	{
		custom_setenv(shell, "OLDPWD", old_pwd);
		custom_setenv(shell, "PWD", new_pwd);
		free(new_pwd);
	}
	free(old_pwd);
}
