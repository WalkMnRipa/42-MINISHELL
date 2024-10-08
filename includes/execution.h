/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 19:06:34 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/07 17:03:10 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

# define HEREDOC_TMP ".heredoc_tmp"

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_cmd
{
	char			**args;
	int				input_fd;
	int				output_fd;
	struct s_cmd	*next;
}					t_cmd;

// Environment functions
t_env				*init_env(char **envp);
char				*get_env_value(t_env *env, const char *key);
void				free_env(t_env *env);

// Command execution functions
void				execute_command(t_cmd *cmd, t_env *env);
char				*find_command_path(const char *command, t_env *env);
void				execute_pipeline(t_cmd *cmd_list, t_env *env);

// Builtin functions
int					is_builtin(char *cmd);
void				execute_builtin(t_cmd *cmd, t_env *env);

// Cleanup and error handling functions
void				free_cmd(t_cmd *cmd);
void				cleanup(t_env *env, t_cmd *cmd);
void				error_exit_message(t_env *env, t_cmd *cmd,
						const char *message);

// Heredoc function
void				handle_heredoc(t_cmd *cmd, char *delimiter);

#endif
