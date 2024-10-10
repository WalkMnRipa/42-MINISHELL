/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 17:46:57 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/10 18:20:27 by jcohen           ###   ########.fr       */
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

# define ERR_UNEXPECTED_NEWLINE "syntax error near unexpected token `newline'\n"
# define ERR_UNEXPECTED_PIPE "syntax error near unexpected token `|'\n"

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_INPUT,
	TOKEN_REDIR_OUTPUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HERE_DOC,
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

t_token				*ft_create_token(char *value, t_token_type type,
						t_quote_type quote_type);
void				add_token(t_token **head, t_token *new_token);
t_token_type		get_token_type(char *value);
t_token				*ft_tokenizer(char *input);

int					modify_token_type(t_token *token, t_token_type new_type);
int					handle_pipe(t_token *current);
int					handle_redir_input(t_token *current);
int					handle_redir_output(t_token *current);
int					handle_redir_append(t_token *current);
int					handle_here_doc(t_token *current);

int					check_syntax_errors(t_token *tokens);
int					handle_operators(t_token **tokens);

void				free_tokens(t_token *tokens);

#endif
