/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_subshell_cmd.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 14:53:14 by mnazarya          #+#    #+#             */
/*   Updated: 2024/02/15 22:02:00 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	execute_subshell_node(t_shell *shell, t_ast_node *node)
{
	pid_t		pid;
	t_ast_node	*sub_node;

	pid = fork();
	if (error(pid == -1, PERROR_MSG, 1, shell))
		return ;
	sub_node = node->node;
	if (pid == 0)
	{
		if (node->red_lst && execute_redir(shell, node->red_lst))
		{
			set_status(shell, 1);
			exit(1);
		}
		if (sub_node->type == AST_SUBSHELL)
		{
			sub_node->in_fd = node->in_fd;
			sub_node->out_fd = node->out_fd;
			execute_subshell_node(shell, sub_node);
		}
		execute(shell, sub_node);
		close_all_fds(shell);
		subshell_exit(shell, &pid);
	}
	waitpid(pid, &(shell->ex_code), 0);
	ex_code_wait(shell);
}
