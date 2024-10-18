/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 15:27:34 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/18 16:45:50 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

// static char	*expand_variable(t_env *env, const char *var_name)
// {
// 	if (ft_strcmp(var_name, "?") == 0)
// 		return (ft_itoa(env->last_exit_status));
// 	else if (ft_strcmp(var_name, "$") == 0)
// 		return (ft_itoa(getpid()));
// 	return (get_env_value(env, var_name));
// }
