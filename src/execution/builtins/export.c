/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 20:01:24 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/02 20:00:50 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/execution.h"

static void	print_export_error(const char *arg)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd((char *)arg, 2);
	ft_putendl_fd("': not a valid identifier", 2);
}

static int	check_var_name_chars(const char *name)
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

static int	is_valid_export_name(const char *name)
{
	char	*equal_sign;
	char	*var_name;
	int		result;

	if (!name || !*name || ft_isdigit(*name))
		return (0);
	if (name[0] == '=')
		return (0);
	equal_sign = ft_strchr(name, '=');
	if (equal_sign)
	{
		if (equal_sign == name)
			return (0);
		var_name = ft_substr(name, 0, equal_sign - name);
		if (!var_name)
			return (0);
		result = check_var_name_chars(var_name);
		free(var_name);
		return (result);
	}
	return (check_var_name_chars(name));
}

static int	handle_export_arg(t_env **env, char *arg)
{
	char	*equal_sign;
	char	temp;

	if (!is_valid_export_name(arg))
	{
		print_export_error(arg);
		return (1);
	}
	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		temp = *equal_sign;
		*equal_sign = '\0';
		custom_setenv(env, arg, equal_sign + 1);
		*equal_sign = temp;
	}
	else
		custom_setenv(env, arg, "");
	return (0);
}

void	builtin_export(t_cmd *cmd, t_env **env, char **args)
{
	int	i;

	cmd->exit_status = 0;
	if (!env || !*env || !args)
		return ;
	i = 1;
	while (args[i])
	{
		if (handle_export_arg(env, args[i]))
			cmd->exit_status = 1;
		i++;
	}
}
