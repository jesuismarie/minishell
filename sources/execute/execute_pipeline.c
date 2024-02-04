/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 06:37:33 by mnazarya          #+#    #+#             */
/*   Updated: 2024/02/04 14:23:42 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	fds_helper(t_ast_node *node, int in_fd, int out_fd)
{
	t_cmd		*cmd;
	t_pipe		*pipe;
	t_operator	*op;

	if (node->type == AST_COMMAND)
	{
		cmd = node->node;
		cmd->in_fd = in_fd;
		cmd->out_fd = out_fd;
	}
	else if (node->type == AST_PIPE)
	{
		pipe = node->node;
		pipe->in_fd = in_fd;
		pipe->out_fd = out_fd;
	}
	else if (node->type == AST_LOGICAL_OP)
	{
		op = node->node;
		op->in_fd = in_fd;
		op->out_fd = out_fd;
	}
}

static void	set_subshell_fds(t_ast_node *sub_node, int in_fd, int out_fd)
{
	t_ast_node	*tmp;

	if (sub_node->type == AST_SUBSHELL)
	{
		tmp = sub_node->node;
		set_subshell_fds(tmp, in_fd, out_fd);
	}
	else
		fds_helper(sub_node, in_fd, out_fd);
}

static void	set_fds(t_pipe *node)
{
	t_ast_node	*tmp;
	int			fds[2];

	if (pipe(fds) == -1)
	{
		perror(PERROR_MSG);
		exit(1);
	}
	if (node->left->type == AST_SUBSHELL)
	{
		tmp = node->left->node;
		set_subshell_fds(tmp, node->in_fd, fds[1]);
	}
	else if (node->right->type == AST_SUBSHELL)
	{
		tmp = node->left->node;
		set_subshell_fds(tmp, fds[0], node->out_fd);
	}
	else
	{
		fds_helper(node->left, node->in_fd, fds[1]);
		fds_helper(node->right, fds[0], node->out_fd);
	}
}

void	execute_pipeline(t_shell *shell, t_ast_node *node)
{
	t_pipe	*pipe;

	pipe = node->node;
	if (pipe->in_fd == -2)
		pipe->in_fd = STDIN_FILENO;
	if (pipe->out_fd == -2)
		pipe->out_fd = STDOUT_FILENO;
	set_fds(pipe);
	execute(shell, pipe->left);
	execute(shell, pipe->right);
}
