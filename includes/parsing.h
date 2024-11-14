/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 17:46:57 by jcohen            #+#    #+#             */
/*   Updated: 2024/11/14 15:17:34 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

/*
** System headers
*/
# include <dirent.h>
# include <fcntl.h>
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

/*
** External library headers
*/
# include <readline/history.h>
# include <readline/readline.h>

/*
** Local headers
*/
# include "../libft/libft.h"
# include "execution.h"

/*
** Error messages
*/
# define ERR_UNEXPECTED_NL "bash: syntax error near unexpected token `newline'"
# define ERR_UNEXPECTED_PIPE "bash: syntax error near unexpected token `|'"
# define ERR_UNEXPECTED_SEMICOL "minishell: syntax error near unexpected token ';'"
# define ERR_UNEXPECTED_TOKEN "bash: syntax error near unexpected token"
# define ERR_MALLOC_FAILED "malloc failed"
# define ERR_INVALID_TOKEN "invalid token"
# define ERR_VARIABLE_NOT_FOUND ": variable not found"

/*
** Token types
*/
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_INPUT,
	TOKEN_REDIR_OUTPUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HERE_DOC,
	TOKEN_VARIABLE,
	TOKEN_ERROR,
}					t_token_type;

/*
** Quote handling
*/
typedef enum e_quote_type
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE,
}					t_quote_type;

typedef struct s_quote_state
{
	t_quote_type	current;
	t_quote_type	previous;
	int				nested_level;
}					t_quote_state;

/*
** Token structure
*/
typedef struct s_token
{
	t_token_type	type;
	char			*value;
	t_quote_type	quote_type;
	struct s_token	*next;
}					t_token;

/*
** Function prototypes
*/

/* Tokenizer functions */
t_token				*create_token(char *value, t_token_type type,
						t_quote_type quote_type);
void				add_token(t_token **head, t_token *new_token);
t_token_type		determine_token_type(char *value);
t_token				*tokenizer(char *input, t_env *env);

/* Token handling functions */
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

/* Quote handling functions */
char				*get_quoted_content(char *input, int start, char quote);
int					get_quote_end(char *input, int start, char quote);
void				init_quote_state(t_quote_state *state);
int					is_quote_char(char c);
int					update_quote_state(t_quote_state *state, char quote_char);

/* Operator handling functions */
int					handle_here_doc(t_token *current);
int					handle_operators(t_token **tokens);
int					modify_token_type(t_token *token, t_token_type new_type);
int					handle_pipe(t_token *current);
int					handle_redir_input(t_token *current);
int					handle_redir_output(t_token *current);
int					handle_redir_append(t_token *current);

/* String manipulation functions */
int					token_handle_space(char *input, int i);
char				*ft_strjoin_free(char *s1, char *s2);
char				*ft_strjoinc(char *str, char c);

/* Syntax and expansion functions */
int					check_syntax_errors(t_token *tokens);
char				*expand_variables_in_str(char *str, t_env *env,
						t_quote_type quote_type);
char				*expand_quoted_word(char *word, t_env *env);

/* Command building functions */
t_cmd				*group_tokens_into_commands(t_token *token_list,
						t_env *env);

/* Cleanup functions */
void				free_tokens(t_token *tokens);
void				free_cmd_list(t_cmd *head);
void				free_string_array(char **array, int count);
void				join_adjacent_word_tokens(t_token **head);
void				join_quoted_word_tokens(t_token **head);

#endif