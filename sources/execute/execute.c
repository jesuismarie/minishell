/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 06:33:11 by mnazarya          #+#    #+#             */
/*   Updated: 2024/02/15 14:32:27 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	execute(t_shell *shell, t_ast_node *node)
{
	if (shell->err == 1)
		return (1);
	if (node->type == AST_COMMAND)
		expand(shell, node);
	if (node->type == AST_LOGICAL_OP)
		execute_log_op(shell, node);
	else if (node->type == AST_PIPE)
		execute_pipeline(shell, node);
	else if (node->type == AST_COMMAND)
		execute_cmd_node(shell, node);
	else if (node->type == AST_SUBSHELL)
		execute_subshell_node(shell, node);
	return (0);
}
