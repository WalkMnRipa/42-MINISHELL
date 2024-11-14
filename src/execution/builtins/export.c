/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 20:01:24 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/14 16:44:25 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/execution.h"

void	print_export_error(const char *arg)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd((char *)arg, 2);
	ft_putendl_fd("': not a valid identifier", 2);
}

static int	handle_export_arg(t_env **env, char *arg)
{
	char	*equal_sign;
	char	*name;
	char	*value;

	if (!is_valid_export_name(arg))
	{
		print_export_error(arg);
		return (1);
	}
	equal_sign = ft_strchr(arg, '=');
	if (!equal_sign)
		return (export_without_value(env, arg));
	name = get_var_name(arg, equal_sign);
	value = equal_sign + 1;
	if (!name)
		return (1);
	if (export_with_value(env, name, value))
	{
		free(name);
		return (1);
	}
	free(name);
	return (0);
}

int	builtin_export(t_cmd *cmd, t_env **env, char **args)
{
	int	i;
	int	exit_status;

	if (!cmd || !env || !*env || !args)
		return (1);
	if (!args[1])
		return (print_sorted_env(*env));
	exit_status = 0;
	i = 1;
	while (args[i])
	{
		if (handle_export_arg(env, args[i]))
			exit_status = 1;
		i++;
	}
	return (exit_status);
}
