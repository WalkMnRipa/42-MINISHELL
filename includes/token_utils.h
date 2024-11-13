/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:17:28 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/13 16:17:35 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_UTILS_H
# define TOKEN_UTILS_H

# include "parsing.h"

typedef struct s_token_state
{
    int in_quotes;       // Are we currently inside quotes?
    char quote_char;     // Which quote character are we using?
    int escape_next;     // Should we escape the next character?
    int word_start;      // Where did the current word start?
    int i;              // Current position in input string
} t_token_state;

// Initialize token state
void init_token_state(t_token_state *state);

// Check if character is a token delimiter
int is_token_delimiter(char c);

// Handle quoted strings
int handle_quotes(char *input, t_token_state *state, t_token **head, t_env *env);

// Debug functions
void debug_print_token(t_token *token);
void debug_print_token_list(t_token *head);

#endif