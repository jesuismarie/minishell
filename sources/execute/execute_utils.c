/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 05:31:17 by mnazarya          #+#    #+#             */
/*   Updated: 2024/02/08 07:09:31 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	subshell_wait(t_shell *shell, t_ast_node *node, int *pid)
{
	waitpid(*pid, &(shell->ex_code), 0);
	if (WIFEXITED(shell->ex_code))
		set_status(shell, WEXITSTATUS(shell->ex_code));
	else
		set_status(shell, WTERMSIG(shell->ex_code));
	close_fds(node->red_lst);
}

void	close_fds(t_redir *red_lst)
{
	t_redir	*tmp;

	tmp = red_lst;
	while (tmp)
	{
		close(tmp->in_fd);
		close(tmp->out_fd);
		tmp = tmp->next;
	}
}
