/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 20:01:56 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/17 20:29:34 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	is_numeric_arg(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	handle_numeric_argument(const char *arg)
{
	long	num;
	char	*endptr;

	num = strtol(arg, &endptr, 10);
	if (*endptr != '\0' || !is_numeric_arg(arg))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd((char *)arg, STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		return (2);
	}
	return ((unsigned char)num);
}

int	builtin_exit(t_cmd *cmd, t_env **env, char **args)
{
	int	status;

	ft_putendl_fd("exit", STDOUT_FILENO);
	if (!args || !cmd || !env)
		return (1);
	status = cmd->exit_status;
	if (args[1])
	{
		if (!is_numeric_arg(args[1]) || ft_strlen(args[1]) > 20)
		{
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(args[1], STDERR_FILENO);
			ft_putendl_fd(": numeric argument required", STDERR_FILENO);
			cleanup_all(*env, cmd, 2);
			return (2);
		}
		status = handle_numeric_argument(args[1]);
		if (args[2])
			return ((ft_putendl_fd("minishell: exit: too many arguments",
						STDERR_FILENO), cmd->exit_status = 1), 1);
	}
	cmd->exit_status = status & 0xFF;
	cleanup_all(*env, cmd, cmd->exit_status);
	return (status & 0xFF);
}
