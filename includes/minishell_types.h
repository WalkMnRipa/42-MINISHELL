/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_types.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 13:46:36 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/19 13:47:08 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_TYPES_H
# define MINISHELL_TYPES_H

/* Token types and structures */
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_INPUT,
	TOKEN_REDIR_OUTPUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HERE_DOC,
	TOKEN_VARIABLE,
	TOKEN_ERROR
}					t_token_type;

typedef enum e_quote_type
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE
}					t_quote_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	t_quote_type	quote_type;
	struct s_token	*next;
}					t_token;

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

/* Pipeline state structure */
typedef struct s_pipe_info
{
	int				index;
	int				current_pipe;
	int				cmd_count;
}					t_pipe_info;

#endif