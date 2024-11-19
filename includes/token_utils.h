/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:17:28 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/19 00:57:28 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_UTILS_H
# define TOKEN_UTILS_H

# include "parsing.h"

/*
** Process state structure
*/
typedef struct s_process_state
{
	char	*input;
	t_token	**head;
	t_env	*env;
	char	*processed_input;
	int		in_quotes;
	char	quote_char;
}			t_process_state;

/*
** Token processing utilities
*/
typedef struct s_quote_processor
{
	char	*input;
	char	*result;
	int		i;
	int		in_quotes;
	char	quote_char;
}			t_quote_processor;

typedef struct s_var_processor
{
	char	*input;
	char	*result;
	t_env	*env;
	int		i;
	int		in_quotes;
}			t_var_processor;

typedef struct s_operator_processor
{
	char	*input;
	t_token	**head;
	int		i;
}			t_operator_processor;

typedef struct s_word_processor
{
	char	*input;
	t_token	**head;
	t_env	*env;
	int		i;
	int		start;
}			t_word_processor;

/*
** Function prototypes
*/
void		init_process_state(t_process_state *ps, char *input, t_token **head,
				t_env *env);
t_token		*create_token(char *value, t_token_type type,
				t_quote_type quote_type);
void		add_token(t_token **head, t_token *new_token);

/* Quote processing */
void		init_quote_processor(t_quote_processor *qp, char *input);
int			is_quote(char c);
char		*process_quotes(char *input);

/* Variable processing */
void		init_var_processor(t_var_processor *vp, char *input, t_env *env);
char		*process_variables(char *input, t_env *env, int in_quotes);

/* Operator processing */
void		init_operator_processor(t_operator_processor *op, char *input,
				t_token **head);
int			is_operator(char c);
int			get_operator_length(char *str, int start);
int			process_operators(t_process_state *ps);

/* Word processing */
void		init_word_processor(t_word_processor *wp, char *input,
				t_token **head, t_env *env);
int			is_word_char(char c);
char		*get_word(char *input, int start, int end);
int			process_words(t_process_state *ps);

/* Debug functions */
void		debug_print_token(t_token *token);
void		debug_print_token_list(t_token *head);

#endif