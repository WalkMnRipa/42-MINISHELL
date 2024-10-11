/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 20:01:42 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/11 16:25:46 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/execution.h"

void	builtin_unset(t_env **env, char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (!is_valid_env_name(args[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
		}
		else if (is_readonly_var(args[i]))
		{
			ft_putstr_fd("minishell: unset: ", 2);
			ft_putstr_fd(args[i], 2);
			ft_putendl_fd(": readonly variable", 2);
		}
		else
		{
			remove_env_var(env, args[i]);
		}
		i++;
	}
}
