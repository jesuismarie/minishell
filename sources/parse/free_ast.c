/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 11:48:58 by mnazarya          #+#    #+#             */
/*   Updated: 2024/02/02 05:39:13 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	free_redir_node(t_redir **red)
{
	t_redir	*tmp;

	tmp = NULL;
	while (*red)
	{
		tmp = (*red)->next;
		free((*red)->filename->input);
		(*red)->filename->input = NULL;
		free((*red)->filename);
		(*red)->filename = NULL;
		free(*red);
		*red = tmp;
	}
}

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
	free(*node);
	*node = NULL;
}

static void	free_cmd_node(t_ast_node **node)
{
	t_cmd	*cmd;
	t_input	*tmp;

	cmd = (*node)->node;
	if (cmd->name)
	{
		free(cmd->name->input);
		cmd->name->input = NULL;
		free(cmd->name);
		cmd->name = NULL;
	}
	while (cmd->args)
	{
		tmp = cmd->args->next;
		free(cmd->args->input);
		cmd->args->input = NULL;
		free(cmd->args);
		cmd->args = tmp;
	}
	if (cmd->red_lst)
		free_redir_node(&cmd->red_lst);
	free(cmd);
	cmd = NULL;
	free(*node);
	*node = NULL;
}

void	free_ast(t_ast_node **node)
{
	t_ast_node	*tmp;

	if (!(*node))
		return ;
	if ((*node)->type == AST_LOGICAL_OP)
		free_op_node(node);
	else if ((*node)->type == AST_PIPE)
		free_pipe_node(node);
	else if ((*node)->type == AST_SUBSHELL)
	{
		tmp = (*node)->node;
		free_ast(&tmp);
		if ((*node)->red_lst)
			free_redir_node(&((*node)->red_lst));
		free(*node);
	}
	else if ((*node)->type == AST_COMMAND)
		free_cmd_node(node);
}
