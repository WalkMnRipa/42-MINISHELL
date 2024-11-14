/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 20:12:38 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/14 16:46:07 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

/*
** System headers
*/
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/signal.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

/*
** Local headers
*/
# include "../libft/libft.h"

/*
** Global variables
*/
extern int			g_signal_received;

/*
** Error messages
*/
# define ERR_HEREDOC_CREATING "minishell: Failed to create heredoc file"
# define ERR_HEREDOC_OPEN "minishell: heredoc: Failed to open heredoc file"
# define ERR_PIPE_FAILED "minishell: pipe creation failed"
# define ERR_FORK_FAILED "minishell: fork failed"
# define ERR_CMD_NOT_FOUND "minishell: command not found"
# define ERR_NO_SUCH_FILE "minishell: No such file or directory"
# define ERR_PERMISSION_DENIED "minishell: Permission denied"

/*
** File descriptors and prompts
*/
# define HEREDOC_TMP ".heredoc_tmp"
# define HEREDOC_PROMPT "> "

/*
** Data structures
*/

/* Pipeline state structure */
typedef struct s_pipeline_state
{
	int				input_fd;
	int				*pids;
	int				cmd_count;
	int				current_cmd;
}					t_pipeline_state;

/* Environment structure */
typedef struct s_env
{
	char			*key;
	char			*value;
	int				last_exit_status;
	struct s_env	*next;
}					t_env;

/* Command structure */
typedef struct s_cmd
{
	char			**args;
	char			*input_file;
	char			*output_file;
	int				append_output;
	int				input_fd;
	int				output_fd;
	int				exit_status;
	struct s_cmd	*next;
}					t_cmd;

/*
** Function prototypes
*/

/* Pipeline execution functions */
void				execute_pipeline(t_cmd *cmd, t_env **env);
void				execute_command(t_cmd *cmd, t_env **env);
void				execute_single_command(t_cmd *cmd, t_env **env);
void				execute_external_command(t_cmd *cmd, t_env **env);
void				execute_non_builtin(t_cmd *cmd, t_env **env);

/* Exit status handling */
void				update_exit_status(t_cmd *cmd, int status);
void				set_last_exit_status(t_env *env, int status);

/* Command execution utilities */
char				*find_command_path(const char *command, t_env *env);
int					setup_redirections(t_cmd *cmd);
void				restore_std_fds(int stdin_backup, int stdout_backup);

/* Builtin command functions */
int					is_builtin(char *cmd);
void				execute_builtin(t_cmd *cmd, t_env **env);
int					builtin_echo(char **args);
int					builtin_cd(t_cmd *cmd, t_env *env, char **args);
int					builtin_pwd(void);
int					builtin_export(t_cmd *cmd, t_env **env, char **args);
int					builtin_unset(t_cmd *cmd, t_env **env, char **args);
int					builtin_env(t_env *env);
int					builtin_exit(t_cmd *cmd, char **args);

/* export.c */
void				print_export_error(const char *arg);
int					builtin_export(t_cmd *cmd, t_env **env, char **args);

/* export_utils.c */
int					check_var_name_chars(const char *name);
int					is_valid_export_name(const char *name);
char				*get_var_name(const char *arg, const char *equal_sign);
int					export_with_value(t_env **env, const char *name,
						const char *value);
int					export_without_value(t_env **env, const char *arg);

/* export_print.c */
int					print_sorted_env(t_env *env);

/* Environment handling functions */
t_env				*init_env(char **envp);
char				*get_env_value(t_env *env, const char *key);
void				free_env(t_env *env);
int					custom_setenv(t_env **env, const char *name,
						const char *value);
t_env				*create_env_node(char *envp);
int					create_env_var(t_env **env, const char *name,
						const char *value);
void				remove_env_var(t_env **env, const char *name);
int					is_readonly_var(const char *name);
int					is_valid_env_name(const char *name);
char				**env_to_array(t_env *env);

/* Heredoc handling */
int					handle_heredoc(t_cmd *cmd, char *delimiter, t_env *env);
void				update_pwd(t_env *env, char *old_pwd, char *new_pwd);

/* Signal handling */
void				setup_signals(void);
void				reset_signals(void);
void				signal_handler(int signo);

/* Utility functions */
char				*ft_strjoin3(const char *s1, const char *s2,
						const char *s3);
int					is_direct_executable(const char *command);
char				*try_path(const char *dir, const char *command);
void				free_string_array(char **array, int count);

/* Cleanup functions */
void				cleanup(t_env *env, t_cmd *cmd);
void				free_cmd(t_cmd *cmd);
void				free_cmd_list(t_cmd *head);
void				error_exit_message(t_env *env, t_cmd *cmd, char *message);
char				*ft_strtok(char *str, const char *delim);

#endif