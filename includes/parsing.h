/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 17:46:57 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/20 01:48:47 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../libft/libft.h"
# include "execution.h"
# include <dirent.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <term.h>
# include <termios.h>
# include <unistd.h>

# define ERR_UNEXPECTED_NEWLINE "bash: syntax error near unexpected token `newline'"
# define ERR_UNEXPECTED_PIPE "bash: syntax error near unexpected token `|'"
# define ERR_UNEXPECTED_TOKEN "bash: syntax error near unexpected token"
# define ERR_MALLOC_FAILED "malloc failed"
# define ERR_INVALID_TOKEN "invalid token"

# define ERR_SINGLE_QUOTE "Error: Unclosed single quote"
# define ERR_DOUBLE_QUOTE "Error: Unclosed double quote"

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_INPUT,
	TOKEN_REDIR_OUTPUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HERE_DOC,
	TOKEN_VARIABLE,
}					t_token_type;

typedef enum e_quote_type
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE,
}					t_quote_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	t_quote_type	quote_type;
	struct s_token	*next;
}					t_token;

t_token				*create_token(char *value, t_token_type type,
						t_quote_type quote_type);
void				add_token(t_token **head, t_token *new_token);
t_token_type		determine_token_type(char *value);
t_token				*tokenizer(char *input);
int					check_syntax_errors(t_token *tokens);

int					modify_token_type(t_token *token, t_token_type new_type);
int					handle_pipe(t_token *current);
int					handle_redir_input(t_token *current);
int					handle_redir_output(t_token *current);
int					handle_redir_append(t_token *current);
int					handle_here_doc(t_token *current);

int					token_handle_single_quotes(char *input, int i,
						t_token **head);
int					token_handle_double_quotes(char *input, int i,
						t_token **head);
int					check_unclosed_quotes(char *input);

int					token_handle_word(char *input, int i, t_token **head);
int					token_handle_space(char *input, int i);
int					token_handle_redirection(char *input, int i,
						t_token **head);
int					token_handle_variable(char *input, int i, t_token **head);
int					handle_operators(t_token **tokens);

t_cmd				*group_tokens_into_commands(t_token *token_list);

int					is_valid_var_char(char c);
int					is_valid_var_start(char c);

char				*get_var_value(t_env *env, const char *var_name);
char				*replace_var(char *str, int *i, t_env *env);
char				*expand_special_variable(t_env *env, const char *var_name);
char				*get_env_variable(t_env *env, const char *var_name);
char				*expand_variables(char *str, t_env *env);
int					get_var_name_length(char *str);

void				free_tokens(t_token *tokens);
void				free_cmd_list(t_cmd *head);
char				*ft_strjoin_free(char *s1, char *s2);

#endif