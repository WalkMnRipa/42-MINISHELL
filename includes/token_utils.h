/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:17:28 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/14 15:17:36 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_UTILS_H
# define TOKEN_UTILS_H

/*
** Local headers
*/
# include "parsing.h"

/*
** Token state structure
*/
typedef struct s_token_state
{
	int		in_quotes;
	char	quote_char;
	int		escape_next;
	int		word_start;
	int		i;
}			t_token_state;

/*
** Function prototypes
*/

/* Token state functions */
void		init_token_state(t_token_state *state);
int			is_token_delimiter(char c);
int			handle_quotes(char *input, t_token_state *state, t_token **head,
				t_env *env);

/* Debug functions */
void		debug_print_token(t_token *token);
void		debug_print_token_list(t_token *head);

#endif