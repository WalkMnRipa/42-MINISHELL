/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 20:01:42 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/03 16:39:59 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/execution.h"

static int	is_valid_env_name(const char *name)
{
	int	i;

	if (!name || !*name || ft_isdigit(*name))
		return (0);
	i = 0;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	is_readonly_var(const char *name)
{
	const char	*readonly_vars[] = {"PWD", "OLDPWD", NULL};
	int			i;

	i = 0;
	while (readonly_vars[i])
	{
		if (ft_strcmp(name, readonly_vars[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

static int	remove_env_var(char **env, const char *name)
{
	int		i;
	int		j;
	size_t	name_len;

	if (!is_valid_env_name(name))
		return (-1);
	if (is_readonly_var(name))
		return (-2);
	i = 0;
	name_len = ft_strlen(name);
	while (env[i])
	{
		if (ft_strncmp(env[i], name, name_len) == 0 && env[i][name_len] == '=')
		{
			free(env[i]);
			j = i;
			while (env[j])
				env[j++] = env[j + 1];
			return (0);
		}
		i++;
	}
	return (1);
}

void	builtin_unset(t_shell *shell, char **args)
{
	int	i;
	int	result;

	i = 1;
	while (args[i])
	{
		result = remove_env_var(shell->env, args[i]);
		if (result == -1)
		{
			ft_putstr_fd("unset: '", 2);
			ft_putstr_fd(args[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
		}
		else if (result == -2)
		{
			ft_putstr_fd("unset: '", 2);
			ft_putstr_fd(args[i], 2);
			ft_putendl_fd("': read-only variable", 2);
		}
		i++;
	}
}
