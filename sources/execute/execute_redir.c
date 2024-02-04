/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 10:06:13 by mnazarya          #+#    #+#             */
/*   Updated: 2024/02/03 02:09:19 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	set_in(t_shell *shell, t_redir *node)
{
	(void)shell;
	if (node->type == FILE_IN)
		node->out_fd = open(node->filename->input, O_RDONLY);
	if (dup2(node->out_fd, STDIN_FILENO) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(node->filename->input, 2);
		ft_putstr_fd(" :", 2);
		ft_putendl_fd(strerror(errno), 2);
		return (1);
	}
	close(node->out_fd);
	return (0);
}

static int	set_out(t_shell *shell, t_redir *node)
{
	if (node->type == FILE_OUT)
		node->in_fd = open(node->filename->input, \
			O_CREAT | O_TRUNC | O_WRONLY, 0644);
	else
		node->in_fd = open(node->filename->input, \
			O_CREAT | O_APPEND | O_WRONLY, 0644);
	if (node->in_fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(node->filename->input, 2);
		ft_putstr_fd(" :", 2);
		ft_putendl_fd(strerror(errno), 2);
		return (1);
	}
	if (error(dup2(node->in_fd, STDOUT_FILENO) < 0, PERROR_MSG, 1, shell))
		return (1);
	close(node->in_fd);
	return (0);
}

int	setup_redir(t_shell *shell, t_redir *redir)
{
	if (redir->type == FILE_IN || redir->type == HEREDOC)
		return (set_in(shell, redir));
	else if (redir->type == FILE_OUT || redir->type == APPEND)
		return (set_out(shell, redir));
	return (0);
}
