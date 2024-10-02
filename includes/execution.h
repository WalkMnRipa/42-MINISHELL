/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 19:06:34 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/02 18:07:06 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

# define MAX_ARGS 1024

typedef struct s_shell
{
	char	**env;
	char	**cmd_paths;
	char	**cmd_args;
	char	*cmd;
	int		exit_status;
}			t_shell;

void		initialize_shell(t_shell *shell, char **envp);
void		execute_command(t_shell *shell, char *input);
void		cleanup_shell(t_shell *shell);
void		error_exit(t_shell *shell, char *error_message);

// Builtin functions
void		builtin_echo(char **args);
void		builtin_cd(t_shell *shell, char **args);
void		builtin_pwd(void);
void		builtin_export(t_shell *shell, char **args);
void		builtin_unset(t_shell *shell, char **args);
void		builtin_env(t_shell *shell);
void		builtin_exit(t_shell *shell, char **args);
void		update_pwd(t_shell *shell, char *old_pwd, char *new_pwd);

// Utility functions
void		free_string_array(char **arr);
char		*get_cmd(char **paths, char *cmd);
char		*ft_strjoin_shell(char const *s1, char const *s2);
char		*expand_env_vars(t_shell *shell, char *str);

// Signal handling
void		setup_signals(void);

#endif
