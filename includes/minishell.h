/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 18:06:21 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/02 10:58:00 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/signal.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define ERR_HEREDOC_CREATING "minishell: Failed to create heredoc file"
# define ERR_HEREDOC_OPEN "minishell: heredoc: Failed to open heredoc file"
# define ERR_PIPE_FAILED "minishell: pipe creation failed"
# define ERR_FORK_FAILED "minishell: fork failed"
# define ERR_CMD_NOT_FOUND "minishell: command not found"
# define ERR_NO_SUCH_FILE "minishell: No such file or directory"
# define ERR_PERMISSION_DENIED "minishell: Permission denied"

# define HEREDOC_TMP ".heredoc_tmp"
# define HEREDOC_PROMPT "> "

extern int				g_signal_received;

typedef struct s_heredoc
{
	char				*filename;
	char				*delimiter;
	int					expand_vars;
	struct s_heredoc	*next;
}						t_heredoc;

typedef struct s_env
{
	char				*key;
	char				*value;
	int					last_exit_status;
	struct s_env		*next;
}						t_env;

typedef struct s_cmd
{
	char				**args;
	char				*input_file;
	char				*output_file;
	int					input_fd;
	int					output_fd;
	int					append_output;
	int					exit_status;
	t_heredoc			*heredocs;
	struct s_cmd		*next;
}						t_cmd;

typedef struct s_pipe_info
{
	int					cmd_count;
	int					index;
	int					current_pipe;
}						t_pipe_info;

typedef struct s_pipe_data
{
	pid_t				*pids;
	int					pipe_fds[2][2];
	t_pipe_info			*info;
	t_env				**env;
}						t_pipe_data;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_OPERATOR,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF
}						t_token_type;

typedef enum e_quote_state
{
	STATE_NORMAL,
	STATE_SINGLE_QUOTE,
	STATE_DOUBLE_QUOTE
}						t_quote_state;

typedef struct s_token
{
	t_token_type		type;
	char				*value;
	t_quote_state		quote_type;
	struct s_token		*next;
}						t_token;

typedef struct s_quote_data
{
	char				*result;
	char				*str;
	int					i;
	int					j;
	t_quote_state		state;
	char				current_quote;
}						t_quote_data;

/*
** Function prototypes
*/

/* Initialization functions */
t_env					*init_env(char **envp);
t_env					*create_env_node(char *envp);
t_env					*initialize_shell(char **envp);
void					free_env(t_env *env);

/* Environment handling functions */
char					*get_env_value(t_env *env, const char *key);
int						custom_setenv(t_env **env, const char *name,
							const char *value);
int						create_env_var(t_env **env, const char *name,
							const char *value);
int						is_readonly_var(const char *name);
int						is_valid_env_name(const char *name);
char					**env_to_array(t_env *env);

/* Parsing functions */
t_token					*tokenizer(char *input, t_env *env);
t_cmd					*group_tokens_into_commands(t_token *tokens,
							t_env *env);
char					*handle_quotes(char *str, t_env *env);
t_quote_state			get_quote_state(char c, t_quote_state current);
int						is_quote(char c);
int						is_quote_closed(char *str);
char					*expand_variables_in_str(char *str, t_env *env,
							t_quote_state state);
void					print_syntax_error(char *token);
int						check_syntax_errors(t_token *tokens);

/* Quote handling functions */
t_quote_state			get_quote_state(char c, t_quote_state current);
char					*copy_without_quotes(char *str);
char					*handle_quotes(char *str, t_env *env);
int						is_quote(char c);
int						is_quote_closed(char *str);

/* Variable expansion functions */
char					*expand_variables_in_str(char *str, t_env *env,
							t_quote_state state);
char					*expand_single_var(char *str, int *i, t_env *env);
char					*join_expanded_var(char *before, char *var_value,
							char *after, int *i);

/* Command parsing functions */
t_cmd					*create_command(void);
int						add_argument(t_cmd *cmd, char *arg);
int						handle_redirection(t_cmd *cmd, t_token *token,
							t_token *next, t_env *env);
int						handle_output_redirect(t_cmd *cmd, t_token *token,
							t_token *next);
int						handle_input_redirect(t_cmd *cmd, t_token *next);
int						handle_heredoc_with_file(t_cmd *cmd, char *delimiter,
							t_env *env, const char *filename);

/* Token handling functions */
t_token					*create_token(t_token_type type, char *value);
void					add_token(t_token **head, t_token *new_token);
t_token					*get_next_token(char **input, t_env *env);
t_token					*handle_operator(char **input);
t_token					*handle_syntax_check(t_token *head);
void					free_tokens(t_token *head);
void					free_token(t_token *token);
int						is_operator(char c);
int						is_whitespace(char c);
int						is_special_char(char c);

/* Command execution functions */
void					execute_pipeline(t_cmd *cmd, t_env **env);
void					execute_command(t_cmd *cmd, t_env **env);
void					execute_non_builtin(t_cmd *cmd, t_env **env);
void					execute_external_command(t_cmd *cmd, t_env **env);
char					*find_command_path(const char *command, t_env *env);
int						setup_redirections(t_cmd *cmd);
void					update_exit_status(t_cmd *cmd, int status);

/* Pipeline utilities */
void					setup_child_pipes(int pipe_fds[2][2], int i,
							int current_pipe, int has_next);
int						handle_pipe_creation(t_cmd *cmd, int pipe_fds[2][2],
							int current_pipe);
void					handle_parent_pipes(int pipe_fds[2][2], int i,
							int current_pipe);
void					cleanup_pipeline(pid_t *pids, int pipe_fds[2][2],
							int i);
pid_t					create_process(t_cmd *cmd, t_env **env,
							int pipe_fds[2][2], t_pipe_info *info);
void					handle_last_process_status(int status, t_env **env);
void					run_pipeline_loop(t_cmd *cmd, pid_t *pids,
							t_pipe_info *info, t_env **env);

/* Builtin commands */
int						is_builtin(char *cmd);
void					execute_builtin(t_cmd *cmd, t_env **env);
int						builtin_echo(char **args);
int						builtin_cd(t_cmd *cmd, t_env *env, char **args);
int						builtin_pwd(void);
int						builtin_export(t_cmd *cmd, t_env **env, char **args);
int						builtin_unset(t_cmd *cmd, t_env **env, char **args);
int						builtin_env(t_env *env);
int						builtin_exit(t_cmd *cmd, char **args);

/* Export utilities */
void					print_export_error(const char *arg);
int						check_var_name_chars(const char *name);
int						is_valid_export_name(const char *name);
char					*get_var_name(const char *arg, const char *equal_sign);
int						print_sorted_env(t_env *env);
int						export_without_value(t_env **env, const char *arg);
int						export_with_value(t_env **env, const char *name,
							const char *value);

/* Signal handling */
void					setup_signals(void);
void					reset_signals(void);
void					setup_parent_signals(void);

/* Memory management */
void					cleanup(t_env *env, t_cmd *cmd);
void					free_cmd(t_cmd *cmd);
void					free_cmd_list(t_cmd *head);
void					free_string_array(char **array, int count);
void					error_exit_message(t_env *env, t_cmd *cmd,
							char *message);

/* Utility functions */
char					*ft_strjoin3(const char *s1, const char *s2,
							const char *s3);
int						is_direct_executable(const char *command);
char					*try_path(const char *dir, const char *command);
char					*ft_strtok(char *str, const char *delim);
void					update_pwd(t_env *env, char *old_pwd, char *new_pwd);
void					cleanup_fds(int prev_in_fd, int prev_out_fd);
int						check_input_permissions(t_cmd *cmd, int *prev_fds);
int						check_output_permissions(t_cmd *cmd, int *prev_fds);
int						setup_output_fd(t_cmd *cmd, int flags, int *prev_fds);

/* Heredoc */
int						write_heredoc(int fd, char *delimiter, t_env *env,
							int expand_vars);
void					free_heredocs(t_heredoc *heredoc);
t_heredoc				*create_heredoc(char *delimiter);
char					*generate_heredoc_filename(void);
void					handle_heredoc_signal(int sig);
void					setup_heredoc_signals(void);
int						write_heredoc_content(t_heredoc *heredoc, t_env *env);
int						handle_multiple_heredocs(t_cmd *cmd, t_env *env);
int						handle_heredoc_with_file(t_cmd *cmd, char *delimiter,
							t_env *env, const char *filename);

#endif