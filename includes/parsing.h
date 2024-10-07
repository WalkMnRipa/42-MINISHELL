/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 17:46:57 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/07 14:30:49 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../libft/libft.h"
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

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_INPUT,
	TOKEN_REDIR_OUTPUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HERE_DOC,
}						t_token_type;

typedef enum e_quote_type
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE,
}						t_quote_type;

typedef enum e_parse_error
{
	PARSE_ERROR_NONE,
	PARSE_ERROR_SYNTAX,
	PARSE_ERROR_MALLOC,
	PARSE_ERROR_PIPE,
	PARSE_ERROR_REDIR,
}						t_parse_error;

typedef struct s_token
{
	t_token_type		type;
	char				*value;
	t_quote_type		quote_type;
	struct s_token		*next;
}						t_token;

typedef struct s_redirect
{
	t_token_type		type;
	char				*file;
	struct s_redirect	*next;
}						t_redirect;

typedef struct s_command
{
	char				**args;
	t_redirect			*redirects;
	struct s_command	*next;
	int					pipe_out;
}						t_command;

t_token					*ft_create_token(char *value, t_token_type type,
							t_quote_type quote_type);
void					add_token(t_token **head, t_token *new_token);
t_token_type			get_token_type(char *value);
t_token					*ft_tokenizer(char *input);

int						handle_single_quotes(char *input, int i,
							t_token **head);
int						handle_double_quotes(char *input, int i,
							t_token **head);
int						handle_word(char *input, int i, t_token **head);
int						handle_space(char *input, int i);

void					free_tokens(t_token *tokens);

t_parse_error			ft_handle_parse_error(t_parse_error error);

#endif
