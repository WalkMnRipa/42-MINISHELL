/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:37:35 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/18 14:15:09 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"

void	set_last_exit_status(t_env *env, int status)
{
	env->last_exit_status = status;
}
