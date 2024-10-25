/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 20:01:24 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/25 18:07:09 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/execution.h"

void	builtin_export(t_env **env, char **args)
{
	int		i;
	char	*equal_sign;
	char	*name;
	int		has_error;

	if (!env || !*env)
		return ;
	i = 1;
	has_error = 0;
	while (args[i])
	{
		equal_sign = ft_strchr(args[i], '=');
		if (equal_sign)
		{
			*equal_sign = '\0';
			name = args[i];
			if (!is_valid_env_name(name))
			{
				ft_putstr_fd("minishell: export: `", 2);
				ft_putstr_fd(args[i], 2);
				ft_putendl_fd("': not a valid identifier", 2);
				has_error = 1;
				*equal_sign = '=';
				i++;
				continue ;
			}
			custom_setenv(env, args[i], equal_sign + 1);
			*equal_sign = '=';
		}
		else if (!is_valid_env_name(args[i]))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
			has_error = 1;
		}
		else
			custom_setenv(env, args[i], "");
		i++;
	}
	if (has_error)
		exit(1);
}
