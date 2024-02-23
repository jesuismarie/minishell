/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 06:37:33 by mnazarya          #+#    #+#             */
/*   Updated: 2024/02/20 16:57:23 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	fds_helper(t_shell *shell, t_ast_node *node, int in, int out)
{
	t_cmd		*cmd;
	t_pipe		*pipe;

	if (node->type == AST_COMMAND)
	{
		cmd = node->node;
		cmd->in_fd = in;
		cmd->out_fd = out;
		if (cmd->in_fd < FOPEN_MAX)
			shell->all_fds[shell->index++] = cmd->in_fd;
		if (cmd->out_fd < FOPEN_MAX)
			shell->all_fds[shell->index++] = cmd->out_fd;
	}
	else if (node->type == AST_PIPE)
	{
		pipe = node->node;
		pipe->in_fd = in;
		pipe->out_fd = out;
		if (pipe->in_fd < FOPEN_MAX)
			shell->all_fds[shell->index++] = pipe->in_fd;
		if (pipe->out_fd < FOPEN_MAX)
			shell->all_fds[shell->index++] = pipe->out_fd;
	}
}

static void	set_subshell_fds(t_shell *shell, t_ast_node *sub, int in, int out)
{
	sub->in_fd = in;
	sub->out_fd = out;
	if (sub->in_fd < FOPEN_MAX)
		shell->all_fds[shell->index++] = sub->in_fd;
	if (sub->out_fd < FOPEN_MAX)
		shell->all_fds[shell->index++] = sub->out_fd;
}

static void	set_fds(t_shell *shell, t_pipe *node)
{
	int	fds[2];

	if (pipe(fds) == -1)
	{
		perror(PERROR_MSG);
		shell->err = 1;
		return ;
	}
	if (node->left->type == AST_SUBSHELL)
		set_subshell_fds(shell, node->left, node->in_fd, fds[1]);
	else
		fds_helper(shell, node->left, node->in_fd, fds[1]);
	if (node->right->type == AST_SUBSHELL)
		set_subshell_fds(shell, node->right, fds[0], node->out_fd);
	else
		fds_helper(shell, node->right, fds[0], node->out_fd);
}

void	execute_pipeline(t_shell *shell, t_ast_node *node)
{
	t_pipe	*pipe;

	pipe = node->node;
	if (pipe->in_fd == -2)
		pipe->in_fd = STDIN_FILENO;
	if (pipe->out_fd == -2)
		pipe->out_fd = STDOUT_FILENO;
	set_fds(shell, pipe);
	shell->flag = 1;
	execute(shell, pipe->left);
	shell->flag = 1;
	execute(shell, pipe->right);
	shell->flag = 0;
}
