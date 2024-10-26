/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 17:46:57 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/26 16:25:55 by jcohen           ###   ########.fr       */
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

# define ERR_UNEXPECTED_NL "bash: syntax error near unexpected token `newline'"
# define ERR_UNEXPECTED_PIPE "bash: syntax error near unexpected token `|'"
# define ERR_UNEXPECTED_TOKEN "bash: syntax error near unexpected token"
# define ERR_MALLOC_FAILED "malloc failed"
# define ERR_INVALID_TOKEN "invalid token"
# define ERR_VARIABLE_NOT_FOUND ": variable not found"

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

// tokenizer.c
t_token				*create_token(char *value, t_token_type type,
						t_quote_type quote_type);
void				add_token(t_token **head, t_token *new_token);
t_token_type		determine_token_type(char *value);
t_token				*tokenizer(char *input, t_env *env);

// token_handlers.c
int					token_handle_single_quotes(char *input, int i,
						t_token **head);
int					token_handle_double_quotes(char *input, int i,
						t_token **head, t_env *env);
int					token_handle_word(char *input, int i, t_token **head,
						t_env *env);
int					token_handle_redirection(char *input, int i,
						t_token **head);
int					token_handle_variable(char *input, int i, t_token **head,
						t_env *env);

// token_handlers_utils.c
char				*get_quoted_content(char *input, int start, char quote);
int					get_quote_end(char *input, int start, char quote);
char				*get_quoted_content(char *input, int start, char quote);

// operator_utils.c
int					handle_here_doc(t_token *current);
int					handle_operators(t_token **tokens);

// operators_handlers.c
int					modify_token_type(t_token *token, t_token_type new_type);
int					handle_pipe(t_token *current);
int					handle_redir_input(t_token *current);
int					handle_redir_output(t_token *current);
int					handle_redir_append(t_token *current);

// utils.c
int					token_handle_space(char *input, int i);
char				*ft_strjoin_free(char *s1, char *s2);
char				*ft_strjoinc(char *str, char c);

// syntax_checker.c
int					check_syntax_errors(t_token *tokens);

// expansion.c
char				*expand_variables_in_str(char *str, t_env *env,
						t_quote_type quote_type);
char				*expand_quoted_word(char *word, t_env *env);

// command_builder.c
t_cmd				*group_tokens_into_commands(t_token *token_list,
						t_env *env);

// cleanup.c
void				free_tokens(t_token *tokens);
void				free_cmd_list(t_cmd *head);
void				free_string_array(char **array, int count);

#endif