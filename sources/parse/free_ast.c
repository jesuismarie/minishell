/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 11:48:58 by mnazarya          #+#    #+#             */
/*   Updated: 2024/01/14 12:18:14 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	free_op_node(t_ast_node **node)
{
	t_operator	*op;

	op = (*node)->node;
	free_ast(&op->left);
	op->left = NULL;
	free_ast(&op->right);
	op->right = NULL;
	free(op);
	op = NULL;
	if ((*node)->next)
		free_ast(&(*node)->next);
	free(*node);
	*node = NULL;
}

static void	free_pipe_node(t_ast_node **node)
{
	t_pipe		*pipe;

	pipe = (*node)->node;
	free_ast(&pipe->left);
	pipe->left = NULL;
	free_ast(&pipe->right);
	pipe->right = NULL;
	free(pipe);
	pipe = NULL;
	if ((*node)->next)
		free_ast(&(*node)->next);
	free(*node);
	*node = NULL;
}

static void	free_cmd_node(t_ast_node **node)
{
	t_cmd	*cmd;
	t_input	*tmp;

	cmd = (*node)->node;
	free(cmd->name->input);
	cmd->name->input = NULL;
	free(cmd->name);
	cmd->name = NULL;
	while (cmd->args)
	{
		tmp = cmd->args->next;
		free(cmd->args->input);
		cmd->args->input = NULL;
		free(cmd->args);
		cmd->args = tmp;
	}
	free(cmd);
	cmd = NULL;
	if ((*node)->next)
		free_ast(&(*node)->next);
	free(*node);
	*node = NULL;
}

static void	free_redir_node(t_ast_node **node)
{
	t_redir	*red;
	t_redir	*tmp;

	tmp = NULL;
	red = (*node)->node;
	while (red)
	{
		tmp = red->next;
		free(red->filename->input);
		red->filename->input = NULL;
		free(red->filename);
		red->filename = NULL;
		free(red);
		red = tmp;
	}
	if ((*node)->next)
		free_ast(&(*node)->next);
	free(*node);
	*node = NULL;
}

void	free_ast(t_ast_node **node)
{
	if (!(*node))
		return ;
	if ((*node)->type == AST_LOGICAL_OP)
		free_op_node(node);
	else if ((*node)->type == AST_PIPE)
		free_pipe_node(node);
	else if ((*node)->type == AST_REDIRECTION)
		free_redir_node(node);
	else if ((*node)->type == AST_COMMAND)
		free_cmd_node(node);
}
