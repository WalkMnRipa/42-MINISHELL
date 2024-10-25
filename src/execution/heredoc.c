/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:38:41 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/25 23:16:05 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"
#include "../../includes/parsing.h"
#include <signal.h>
#include <sys/signal.h>
#include <sys/types.h>

static void	handle_heredoc_signals(int signo)
{
	if (signo == SIGINT)
	{
		g_signal_received = SIGINT;
		close(STDIN_FILENO);
		ft_putchar_fd('\n', STDERR_FILENO);
	}
}

static void	setup_heredoc_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handle_heredoc_signals;
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

static char	*expand_heredoc_line(char *line, t_env *env, int expand_vars)
{
	char	*expanded;
	char	*result;

	if (!expand_vars || !line)
		return (ft_strdup(line));
	expanded = expand_variables_in_str(line, env, QUOTE_NONE);
	if (!expanded)
		return (NULL);
	// Si la ligne n'a pas de newline à la fin, on l'ajoute
	if (expanded[0] != '\0' && expanded[ft_strlen(expanded) - 1] != '\n')
	{
		result = ft_strjoin(expanded, "\n");
		free(expanded);
		return (result);
	}
	return (expanded);
}

static int	write_heredoc_content(int tmp_fd, char *delimiter, t_env *env,
		int expand_vars)
{
	char	*line;
	char	*expanded_line;

	while (1)
	{
		ft_putstr_fd("> ", STDERR_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (!line || g_signal_received == SIGINT)
		{
			free(line);
			return (g_signal_received == SIGINT ? 130 : 0);
		}
		// Vérifie le délimiteur sans le \n final
		if (ft_strlen(line) > 0 && line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		// Restaure le \n pour la ligne
		if (ft_strlen(line) > 0)
			ft_strjoinc(line, '\n');
		expanded_line = expand_heredoc_line(line, env, expand_vars);
		free(line);
		if (expanded_line)
		{
			ft_putstr_fd(expanded_line, tmp_fd);
			free(expanded_line);
		}
	}
	return (0);
}

int	handle_heredoc(t_cmd *cmd, char *delimiter, t_env *env)
{
	int		tmp_fd;
	int		status;
	int		expand_vars;
	char	*clean_delimiter;
	char	tmp_filename[] = "/tmp/heredoc_XXXXXX";

	expand_vars = 1;
	// Traitement des quotes dans le délimiteur
	if (delimiter[0] == '\'' || delimiter[0] == '"')
	{
		expand_vars = 0;
		clean_delimiter = ft_strtrim(delimiter, "'\"");
	}
	else
		clean_delimiter = ft_strdup(delimiter);
	if (!clean_delimiter)
		return (1);
	setup_heredoc_signals();
	// Crée un fichier temporaire unique
	tmp_fd = mkstemp(tmp_filename);
	if (tmp_fd < 0)
	{
		free(clean_delimiter);
		return (1);
	}
	// Supprime le fichier du système de fichiers mais garde le descripteur ouvert
	unlink(tmp_filename);
	status = write_heredoc_content(tmp_fd, clean_delimiter, env, expand_vars);
	free(clean_delimiter);
	if (status != 0)
	{
		close(tmp_fd);
		return (status);
	}
	// Déplace le curseur au début du fichier pour la lecture
	lseek(tmp_fd, 0, SEEK_SET);
	// Assigne le descripteur de fichier à la commande
	cmd->input_fd = tmp_fd;
	return (0);
}
