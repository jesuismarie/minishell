/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_logic_operators.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 12:33:25 by mnazarya          #+#    #+#             */
/*   Updated: 2024/02/15 14:10:31 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_ast_node	*parse_logic_op(t_shell *shell, t_ast_node *left, t_token **tok_lst)
{
	t_ast_node	*node;
	t_operator	*op_node;

	if (*tok_lst && ((*tok_lst)->type == AND_OP || (*tok_lst)->type == OR_OP))
	{
		node = ft_calloc(1, sizeof(t_ast_node));
		error_exit(!node, "malloc", 12);
		op_node = ft_calloc(1, sizeof(t_operator));
		error_exit(!op_node, "malloc", 12);
		node->type = AST_LOGICAL_OP;
		op_node->type = (*tok_lst)->type;
		op_node->left = left;
		*tok_lst = (*tok_lst)->next;
		op_node->right = parse_pipeline(shell, tok_lst);
		node->node = (void *)op_node;
		node->red_lst = NULL;
		node->in_fd = -2;
		node->out_fd = -2;
		return (parse_logic_op(shell, node, tok_lst));
	}
	return (left);
}
