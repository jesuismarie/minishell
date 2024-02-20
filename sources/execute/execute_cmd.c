/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 10:05:58 by mnazarya          #+#    #+#             */
/*   Updated: 2024/02/20 17:15:26 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	execute_cmd_child(t_shell *shell, t_cmd *node, char **cmd)
{
	errno = 0;
	if (dup2(node->in_fd, STDIN_FILENO) == -1 \
	|| dup2(node->out_fd, STDOUT_FILENO) == -1)
	{
		perror(PERROR_MSG);
		exit(1);
	}
	if (node->red_lst && execute_redir(shell, node->red_lst))
		exit(set_status(shell, 1));
	close_all_fds(shell);
	if (node->name && node->name->input)
	{
		execve(node->name->input, cmd, shell->env);
		free_2d(cmd);
		free_2d(shell->env);
		cmd_print_err(node);
	}
	if (errno == ENOENT)
		exit(1);
	exit(0);
}

int	execute_cmd(t_shell *shell, t_ast_node *node)
{
	pid_t	pid;
	char	**cmd;
	t_cmd	*cmd_node;

	cmd = NULL;
	cmd_node = node->node;
	if (cmd_node->name)
	{
		cmd = get_command(cmd_node);
		if (!ft_strchr(cmd_node->name->input, '/'))
			cmd_node->name->input = find_cmd_abs_path(shell, cmd_node);
	}
	pid = fork();
	if (error(pid == -1, PERROR_MSG, 1, shell))
	{
		shell->err = 1;
		return (free_2d(cmd), 1);
	}
	if (pid == 0)
		execute_cmd_child(shell, cmd_node, cmd);
	return (free_2d(cmd), 0);
}

void	execute_cmd_node(t_shell *shell, t_ast_node *node)
{
	t_cmd	*cmd;

	cmd = node->node;
	if (cmd->in_fd == -2)
		cmd->in_fd = STDIN_FILENO;
	if (cmd->out_fd == -2)
		cmd->out_fd = STDOUT_FILENO;
	if (!check_builtins(node->node))
		do_builtin_in_pipe(shell, node);
	else
		execute_cmd(shell, node);
	if (cmd->in_fd != STDIN_FILENO)
		close(cmd->in_fd);
	if (cmd->out_fd != STDOUT_FILENO)
		close(cmd->out_fd);
}
