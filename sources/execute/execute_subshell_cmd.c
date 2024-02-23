/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_subshell_cmd.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 14:53:14 by mnazarya          #+#    #+#             */
/*   Updated: 2024/02/18 19:57:49 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	set_sub_helper(t_shell *shell, t_ast_node *node)
{
	t_operator	*op;

	if (node->type == AST_LOGICAL_OP)
	{
		op = node->node;
		op->in_fd = node->in_fd;
		op->out_fd = node->out_fd;
		if (op->in_fd < FOPEN_MAX)
			shell->all_fds[shell->index++] = op->in_fd;
		if (op->out_fd < FOPEN_MAX)
			shell->all_fds[shell->index++] = op->out_fd;
	}
}

static void	set_sub_fds(t_shell *shell, t_ast_node *node)
{
	t_cmd		*cmd;
	t_pipe		*pipe;

	if (node->type == AST_COMMAND)
	{
		cmd = node->node;
		cmd->in_fd = node->in_fd;
		cmd->out_fd = node->out_fd;
		if (cmd->in_fd < FOPEN_MAX)
			shell->all_fds[shell->index++] = cmd->in_fd;
		if (cmd->out_fd < FOPEN_MAX)
			shell->all_fds[shell->index++] = cmd->out_fd;
	}
	else if (node->type == AST_PIPE)
	{
		pipe = node->node;
		pipe->in_fd = node->in_fd;
		pipe->out_fd = node->out_fd;
		if (pipe->in_fd < FOPEN_MAX)
			shell->all_fds[shell->index++] = pipe->in_fd;
		if (pipe->out_fd < FOPEN_MAX)
			shell->all_fds[shell->index++] = pipe->out_fd;
	}
	else
		set_sub_helper(shell, node);
}

static void	execute_subshell_helper(t_shell *shell, t_ast_node *node)
{
	t_ast_node	*sub_node;

	sub_node = node->node;
	if (dup2(node->in_fd, STDIN_FILENO) == -1 \
	|| dup2(node->out_fd, STDOUT_FILENO) == -1)
	{
		perror(PERROR_MSG);
		exit(1);
	}
	if (node->red_lst && execute_redir(shell, node->red_lst))
	{
		set_status(shell, 1);
		exit(1);
	}
	if (sub_node->type == AST_SUBSHELL)
	{
		sub_node->in_fd = node->in_fd;
		sub_node->out_fd = node->out_fd;
		execute_subshell_node(shell, sub_node);
	}
	else
	{
		set_sub_fds(shell, node);
		execute(shell, sub_node);
	}
}

void	execute_subshell_node(t_shell *shell, t_ast_node *node)
{
	pid_t		pid;

	if (node->in_fd == -2)
		node->in_fd = shell->all_fds[0];
	if (node->out_fd == -2)
		node->out_fd = shell->all_fds[1];
	pid = fork();
	if (error(pid == -1, PERROR_MSG, 1, shell))
		return ;
	if (pid == 0)
	{
		execute_subshell_helper(shell, node);
		close_all_fds(shell);
		subshell_exit(shell, &pid);
	}
	waitpid(pid, &(shell->ex_code), 0);
	ex_code_wait(shell);
}
