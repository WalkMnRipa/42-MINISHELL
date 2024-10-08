/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:22:03 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/08 17:17:01 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "execution.h"
# include "parsing.h"

typedef struct s_shell_data
{
	char		**envp;
	int			last_exit_status;
	int			is_running;
	t_command	*commands;
}				t_shell_data;

typedef

	void
	initialize_shell_data(t_shell_data *shell_data, char **envp);
void			cleanup_shell(t_shell_data *shell_data);
void			cleanup_iteration(t_shell_data *shell_data);
void			add_history(char *input);

#endif
