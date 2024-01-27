/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 06:33:11 by mnazarya          #+#    #+#             */
/*   Updated: 2024/01/27 07:51:56 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	execute(t_shell *shell, t_ast_node *node)
{
	t_cmd	*cmd;

	if (node)
	{
		cmd = node->node;
		if (node->type == AST_COMMAND)
		{
			expand(shell, node);
			call_builtins(shell, cmd);
		}
	}
	if (node->type == AST_LOGICAL_OP)
		execute_log_op(shell, node);
	// else if (node->type == AST_PIPE)
	// 	execute_pipeline(shell, node);
	// else if (node->type == AST_REDIRECTION)
	// 	execute_redir();
	// else if (node->type == AST_COMMAND)
	// 	execute_cmd();
	return (0);
}
