/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 19:31:10 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/20 01:52:36 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parsing.h"

int	get_var_name_length(char *str)
{
	int	i;

	i = 0;
	while (str[i] && is_valid_var_char(str[i]))
		i++;
	return (i);
}

char	*replace_var(char *str, int *i, t_env *env)
{
	int		var_length;
	char	*var_name;
	char	*var_value;

	var_length = get_var_name_length(str);
	var_name = ft_substr(str, 0, var_length);
	var_value = get_var_value(env, var_name);
	free(var_name);
	return (var_value);
}
