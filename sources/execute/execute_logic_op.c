/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_logic_op.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 05:36:23 by mnazarya          #+#    #+#             */
/*   Updated: 2024/02/15 16:44:52 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	execute_log_op(t_shell *shell, t_ast_node *node)
{
	t_operator	*log_op;

	log_op = node->node;
	execute(shell, log_op->left);
	while (wait(&(shell->ex_code)) != -1)
		;
	ex_code_wait(shell);
	if (!shell->ex_code && log_op->type == AND_OP)
		execute(shell, log_op->right);
	else if (shell->ex_code && log_op->type == OR_OP)
		execute(shell, log_op->right);
}
