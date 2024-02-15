/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 05:31:17 by mnazarya          #+#    #+#             */
/*   Updated: 2024/02/15 19:42:14 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	cmd_print_err(t_cmd *node)
{
	if (errno == ENOENT)
	{
		ft_putstr_fd(node->name->input, 2);
		ft_putendl_fd(": command not found", 2);
	}
	else if (errno == EACCES)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(node->name->input, 2);
		ft_putendl_fd(": is a directory", 2);
		exit(126);
	}
	else
		perror(PERROR_MSG);
	exit(127);
}

void	ex_code_wait(t_shell *shell)
{
	if (WTERMSIG(shell->ex_code) == SIGINT \
	|| WTERMSIG(shell->ex_code) == SIGQUIT)
		set_status(shell, WTERMSIG(shell->ex_code) + 128);
	else
		set_status(shell, WEXITSTATUS(shell->ex_code));
}

void	subshell_exit(t_shell *shell, int *pid)
{
	waitpid(*pid, &(shell->ex_code), 0);
	if (WIFEXITED(shell->ex_code))
		exit(WEXITSTATUS(shell->ex_code));
	else
		exit(WTERMSIG(shell->ex_code));
}

void	close_all_fds(t_shell *shell)
{
	int	i;

	i = 0;
	while (i < shell->index)
	{
		if (shell->all_fds[i] != STDIN_FILENO \
		&& shell->all_fds[i] != STDOUT_FILENO \
		&& shell->all_fds[i] != STDERR_FILENO)
		{
			close(shell->all_fds[i]);
			shell->all_fds[i] = -1;
		}
		i++;
	}
}
