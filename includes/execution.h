/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 20:12:38 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/25 16:18:34 by ggaribot         ###   ########.fr       */
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
# include "init.h"
# include "minishell_types.h"
# include "parsing.h"

/*
** Global variables
*/
extern int	g_signal_received;

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

/* Pipeline functions */
void		execute_pipeline(t_cmd *cmd, t_env **env);
void		execute_non_builtin(t_cmd *cmd, t_env **env);
void		execute_external_command(t_cmd *cmd, t_env **env);
void		execute_command(t_cmd *cmd, t_env **env);
void		handle_last_process_status(int status, t_env **env);
void		setup_child_pipes(int pipe_fds[2][2], int i, int current_pipe,
				int has_next);
int			handle_pipe_creation(t_cmd *cmd, int pipe_fds[2][2],
				int current_pipe);
void		handle_parent_pipes(int pipe_fds[2][2], int i, int current_pipe);
void		cleanup_pipeline(pid_t *pids, int pipe_fds[2][2], int i);
void		run_pipeline_loop(t_cmd *cmd, pid_t *pids, t_pipe_info *info,
				t_env **env);
pid_t		create_process(t_cmd *cmd, t_env **env, int pipe_fds[2][2],
				t_pipe_info *info);

/* Exit status handling */
void		set_last_exit_status(t_env *env, int status);
void		update_exit_status(t_cmd *cmd, int status);

/* Command execution utilities */
char		*find_command_path(const char *command, t_env *env);
int			setup_redirections(t_cmd *cmd);
void		restore_std_fds(int stdin_backup, int stdout_backup);

/* Builtin command functions */
int			is_builtin(char *cmd);
void		execute_builtin(t_cmd *cmd, t_env **env);
int			builtin_echo(char **args);
int			builtin_cd(t_cmd *cmd, t_env *env, char **args);
int			builtin_pwd(void);
int			builtin_export(t_cmd *cmd, t_env **env, char **args);
int			builtin_unset(t_cmd *cmd, t_env **env, char **args);
int			builtin_env(t_env *env);
int			builtin_exit(t_cmd *cmd, char **args);

/* export.c */
void		print_export_error(const char *arg);
int			builtin_export(t_cmd *cmd, t_env **env, char **args);

/* export_utils.c */
int			check_var_name_chars(const char *name);
int			is_valid_export_name(const char *name);
char		*get_var_name(const char *arg, const char *equal_sign);
int			export_with_value(t_env **env, const char *name, const char *value);
int			export_without_value(t_env **env, const char *arg);

/* export_print.c */
int			print_sorted_env(t_env *env);

/* Environment handling functions */
char		*get_env_value(t_env *env, const char *key);
int			custom_setenv(t_env **env, const char *name, const char *value);
int			create_env_var(t_env **env, const char *name, const char *value);
int			is_readonly_var(const char *name);
int			is_valid_env_name(const char *name);
char		**env_to_array(t_env *env);

/* Heredoc handling */
int			handle_heredoc(t_cmd *cmd, char *delimiter, t_env *env);
void		update_pwd(t_env *env, char *old_pwd, char *new_pwd);

/* Signal handling */
void		setup_signals(void);
void		reset_signals(void);
void		setup_parent_signals(void);

/* Utility functions */
char		*ft_strjoin3(const char *s1, const char *s2, const char *s3);
int			is_direct_executable(const char *command);
char		*try_path(const char *dir, const char *command);
void		free_string_array(char **array, int count);

/* Cleanup functions */
void		cleanup(t_env *env, t_cmd *cmd);
void		free_cmd(t_cmd *cmd);
void		free_cmd_list(t_cmd *head);
void		error_exit_message(t_env *env, t_cmd *cmd, char *message);
char		*ft_strtok(char *str, const char *delim);

#endif