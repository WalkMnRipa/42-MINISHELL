/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 17:46:57 by jcohen            #+#    #+#             */
/*   Updated: 2024/11/19 00:57:15 by ggaribot         ###   ########.fr       */
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

/*
** Token types and structures
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

/*
** Function prototypes for token processing
*/
t_token				*tokenizer(char *input, t_env *env);
char				*expand_variables_in_str(char *str, t_env *env,
						t_quote_type quote_type);
void				free_tokens(t_token *tokens);
t_cmd				*group_tokens_into_commands(t_token *token_list,
						t_env *env);

# include "token_utils.h"

#endif