/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:32:07 by jcohen            #+#    #+#             */
/*   Updated: 2024/11/18 15:31:40 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

int	is_special_parameter(char c)
{
	return (c == '?' || c == '$');
}

char	*handle_special_parameter(char c, t_env *env)
{
	if (c == '?')
		return (ft_itoa(env->last_exit_status));
	else if (c == '$')
		return (ft_itoa(getpid()));
	return (ft_strdup(""));
}
