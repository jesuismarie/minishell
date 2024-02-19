/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_logic_op.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 05:36:23 by mnazarya          #+#    #+#             */
/*   Updated: 2024/02/18 19:49:38 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	execute_log_op(t_shell *shell, t_ast_node *node)
{
	t_operator	*log_op;

	log_op = node->node;
	if (node->in_fd == -2)
		node->in_fd = shell->all_fds[0];
	if (node->out_fd == -2)
		node->out_fd = shell->all_fds[1];
	dup2(shell->all_fds[0], STDIN_FILENO);
	dup2(shell->all_fds[1], STDOUT_FILENO);
	execute(shell, log_op->left);
	while (wait(&(shell->ex_code)) != -1)
		;
	if (!shell->ex_code && log_op->type == AND_OP)
		execute(shell, log_op->right);
	else if (shell->ex_code && log_op->type == OR_OP)
		execute(shell, log_op->right);
}
