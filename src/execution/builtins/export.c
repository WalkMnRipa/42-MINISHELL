/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 20:01:24 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/10 14:05:35 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/execution.h"

void	builtin_export(t_env **env, char **args)
{
	int		i;
	char	*equal_sign;

	if (!env || !*env)
		return ;
	i = 1;
	while (args[i])
	{
		equal_sign = ft_strchr(args[i], '=');
		if (equal_sign)
		{
			*equal_sign = '\0';
			custom_setenv(env, args[i], equal_sign + 1);
			*equal_sign = '=';
		}
		else
		{
			custom_setenv(env, args[i], "");
		}
		i++;
	}
}
