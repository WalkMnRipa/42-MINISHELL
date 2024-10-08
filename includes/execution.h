/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 19:06:34 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/08 18:56:42 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "../libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <signal.h>
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
	int				exit_status;
	struct s_cmd	*next;
}					t_cmd;

// Environment functions
t_env				*init_env(char **envp);
char				*get_env_value(t_env *env, const char *key);
void				free_env(t_env *env);
int					custom_setenv(t_env **env, const char *name,
						const char *value);
void				remove_env_var(t_env **env, const char *name);

// Command execution functions
void				execute_command(t_cmd *cmd, t_env **env);
char				*find_command_path(const char *command, t_env *env);
void				execute_pipeline(t_cmd *cmd_list, t_env **env);

// Builtin functions
int					is_builtin(char *cmd);
void				execute_builtin(t_cmd *cmd, t_env **env);
void				builtin_cd(t_env *env, char **args);
void				builtin_echo(char **args);
void				builtin_env(t_env *env);
void				builtin_exit(char **args, int *exit_status);
void				builtin_export(t_env **env, char **args);
void				builtin_pwd(void);
void				builtin_unset(t_env **env, char **args);

// Cleanup and error handling functions
void				free_cmd(t_cmd *cmd);
void				cleanup(t_env *env, t_cmd *cmd);
void				error_exit_message(t_env *env, t_cmd *cmd, char *message);

// Heredoc function
void				handle_heredoc(t_cmd *cmd, char *delimiter);

// Update PWD function
void				update_pwd(t_env *env, char *old_pwd, char *new_pwd);

// Additional functions
void				create_pipe(int pipefd[2]);
void				child_process(t_cmd *cmd, t_env **env, int pipefd[2],
						int is_last);
char				*ft_strjoin_array(char **array, char *delimiter);
void				ft_free_array(char **array);
char				*ft_strtok(char *str, const char *delim);

#endif
