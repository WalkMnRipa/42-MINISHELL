/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 13:19:12 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/25 04:27:35 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INIT_H
# define INIT_H

# include "../libft/libft.h"
# include "execution.h"
# include "parsing.h"

t_env	*init_env(char **envp);
t_env	*create_env_node(char *envp);
t_env	*initialize_shell(char **envp);
void	free_env(t_env *env);

#endif