/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:55:53 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/20 00:56:42 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../libft/libft.h"
# include "minishell_types.h"
# include "execution.h"
# include <stdio.h>
# include <stdlib.h>

/* Token types */
typedef enum e_token_type {
    TOKEN_WORD,           // Regular word/command/argument
    TOKEN_OPERATOR,       // Operators like |, >, <, etc.
    TOKEN_PIPE,          // Pipeline operator |
    TOKEN_REDIR_IN,      // Input redirection <
    TOKEN_REDIR_OUT,     // Output redirection >
    TOKEN_REDIR_APPEND,  // Append output >>
    TOKEN_HEREDOC,       // Here document <<
    TOKEN_EOF            // End of input
}   t_token_type;

/* Quote states for parsing */
typedef enum e_quote_state {
    STATE_NORMAL,        // Not in quotes
    STATE_SINGLE_QUOTE,  // Inside single quotes
    STATE_DOUBLE_QUOTE   // Inside double quotes
}   t_quote_state;

/* Token structure */
typedef struct s_token {
    t_token_type    type;
    char            *value;
    t_quote_state   quote_type;
    struct s_token  *next;
}   t_token;

/* Main parsing functions */
t_token     *tokenizer(char *input, t_env *env);
t_cmd       *group_tokens_into_commands(t_token *tokens, t_env *env);

/* Quote handling functions */
char        *handle_quotes(char *str, t_env *env);
t_quote_state get_quote_state(char c, t_quote_state current);
int         is_quote(char c);
int         is_quote_closed(char *str);

/* Variable expansion functions */
char        *expand_variables(char *str, t_env *env, t_quote_state quote_state);
char        *expand_variables_in_str(char *str, t_env *env, t_quote_state state);

/* Token manipulation functions */
t_token     *create_token(t_token_type type, char *value);
void        add_token(t_token **head, t_token *new_token);
t_token     *get_next_token(char **input, t_env *env);
void        free_tokens(t_token *head);
void        free_token(t_token *token);

/* Operator checking functions */
int         is_operator(char c);
int         is_whitespace(char c);
int         is_special_char(char c);

/* Error handling functions */
void        print_syntax_error(char *token);
int         check_syntax_errors(t_token *tokens);

#endif