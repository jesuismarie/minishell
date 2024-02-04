/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 19:06:24 by mnazarya          #+#    #+#             */
/*   Updated: 2024/02/01 13:37:56 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_ast_node	*parse_pipeline(t_shell *shell, t_token **tok_lst)
{
	t_ast_node	*node;

	node = parse_cmd_line(shell, tok_lst);
	return (parse_pipe(shell, node, tok_lst));
}

t_ast_node	*parse_pipe(t_shell *shell, t_ast_node *left, t_token **tok_lst)
{
	t_ast_node	*node;
	t_pipe		*pipe_node;

	if (*tok_lst && (*tok_lst)->type == PIPE_OP)
	{
		node = ft_calloc(sizeof(t_ast_node), 1);
		error_exit(!node, "malloc", 12);
		pipe_node = ft_calloc(sizeof(t_pipe), 1);
		error_exit(!pipe_node, "malloc", 12);
		node->type = AST_PIPE;
		pipe_node->in_fd = -2;
		pipe_node->out_fd = -2;
		pipe_node->left = left;
		*tok_lst = (*tok_lst)->next;
		pipe_node->right = parse_cmd_line(shell, tok_lst);
		node->node = (void *)pipe_node;
		node->red_lst = NULL;
		return (parse_pipe(shell, node, tok_lst));
	}
	return (left);
}
