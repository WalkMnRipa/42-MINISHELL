/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 21:36:33 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/01 21:42:02 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	heredoc_count = 0;

char	*generate_heredoc_filename(int index)
{
	char	*num_str;
	char	*filename;

	num_str = ft_itoa(index);
	if (!num_str)
		return (NULL);
	filename = ft_strjoin(".heredoc_tmp_", num_str);
	free(num_str);
	return (filename);
}

t_heredoc	*create_heredoc(char *delimiter)
{
	t_heredoc	*new_heredoc;

	new_heredoc = malloc(sizeof(t_heredoc));
	if (!new_heredoc)
		return (NULL);
	new_heredoc->delimiter = ft_strdup(delimiter);
	new_heredoc->filename = generate_heredoc_filename(heredoc_count++);
	new_heredoc->expand_vars = (ft_strchr(delimiter, '\'') == NULL);
	new_heredoc->next = NULL;
	if (!new_heredoc->delimiter || !new_heredoc->filename)
	{
		free(new_heredoc->delimiter);
		free(new_heredoc->filename);
		free(new_heredoc);
		return (NULL);
	}
	return (new_heredoc);
}

void	free_heredocs(t_heredoc *heredoc)
{
	t_heredoc	*next;

	while (heredoc)
	{
		next = heredoc->next;
		if (heredoc->filename)
		{
			unlink(heredoc->filename);
			free(heredoc->filename);
		}
		free(heredoc->delimiter);
		free(heredoc);
		heredoc = next;
	}
}

int	write_heredoc_content(t_heredoc *heredoc, t_env *env)
{
	int	fd;
	int	status;

	fd = open(heredoc->filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_putendl_fd(ERR_HEREDOC_CREATING, STDERR_FILENO);
		return (1);
	}
	status = write_heredoc(fd, heredoc->delimiter, env, heredoc->expand_vars);
	close(fd);
	if (status)
	{
		unlink(heredoc->filename);
		return (1);
	}
	return (0);
}

int	handle_multiple_heredocs(t_cmd *cmd, t_env *env)
{
	t_heredoc	*current;
	int			status;

	current = cmd->heredocs;
	while (current)
	{
		status = write_heredoc_content(current, env);
		if (status)
		{
			cmd->exit_status = 1;
			return (1);
		}
		current = current->next;
	}
	// Set the input file to the last heredoc if no explicit input redirection
	if (cmd->heredocs && !cmd->input_file)
	{
		current = cmd->heredocs;
		while (current->next)
			current = current->next;
		cmd->input_file = ft_strdup(current->filename);
		if (!cmd->input_file)
			return (1);
	}
	return (0);
}
