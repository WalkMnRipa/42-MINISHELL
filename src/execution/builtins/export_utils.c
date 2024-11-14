/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:32:11 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/14 16:32:41 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/execution.h"

int	check_var_name_chars(const char *name)
{
	int	i;

	i = 0;
	while (name[i] && name[i] != '=')
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (i > 0);
}

int	is_valid_export_name(const char *name)
{
	if (!name || !*name || ft_isdigit(*name))
		return (0);
	if (name[0] == '=')
		return (0);
	return (check_var_name_chars(name));
}

char	*get_var_name(const char *arg, const char *equal_sign)
{
	int		name_len;
	char	*name;

	name_len = equal_sign - arg;
	name = ft_substr(arg, 0, name_len);
	if (!name)
		return (NULL);
	if (!is_valid_export_name(name))
	{
		free(name);
		return (NULL);
	}
	return (name);
}

int	export_with_value(t_env **env, const char *name, const char *value)
{
	return (custom_setenv(env, name, value) == -1);
}

int	export_without_value(t_env **env, const char *arg)
{
	char	*existing_value;

	existing_value = get_env_value(*env, arg);
	if (!existing_value)
		return (custom_setenv(env, arg, "") == -1);
	return (0);
}
