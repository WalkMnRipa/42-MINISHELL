/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_types.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:56:30 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/25 04:27:37 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_TYPES_H
# define MINISHELL_TYPES_H

# include <unistd.h>

/* Environment variable structure */
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
	char **args;        // Command and its arguments
	char *input_file;   // Input redirection file
	char *output_file;  // Output redirection file
	int input_fd;       // Input file descriptor
	int output_fd;      // Output file descriptor
	int append_output;  // Flag for append mode
	int exit_status;    // Command exit status
	struct s_cmd *next; // Next command in pipeline
}					t_cmd;

/* Pipeline information structure */
typedef struct s_pipe_info
{
	int cmd_count;    // Total number of commands in pipeline
	int index;        // Current command index
	int current_pipe; // Current pipe index (0 or 1)
}					t_pipe_info;

#endif