/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 10:05:58 by mnazarya          #+#    #+#             */
/*   Updated: 2024/02/08 14:52:43 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	execute_cmd_child(t_shell *shell, t_cmd *node, char *path, \
char **cmd)
{
	if (node->red_lst)
		execute_redir(shell, node->red_lst);
	if (dup2(node->in_fd, STDIN_FILENO) == -1 \
	|| dup2(node->out_fd, STDOUT_FILENO) == -1)
	{
		perror(PERROR_MSG);
		exit(1);
	}
	if (node->in_fd != STDIN_FILENO)
		close(node->in_fd);
	if (node->out_fd != STDOUT_FILENO)
		close(node->out_fd);
	execve(path, cmd, shell->env);
	free_2d(cmd);
	free(shell->env);
	if (errno == ENOENT)
	{
		ft_putstr_fd(node->name->input, 2);
		ft_putendl_fd(": command not found", 2);
	}
	else
		perror(PERROR_MSG);
	if (errno == EACCES)
		exit(126);
	exit(127);
}

int	execute_cmd(t_shell *shell, t_ast_node *node)
{
	pid_t	pid;
	char	**cmd;
	t_cmd	*cmd_node;

	cmd_node = node->node;
	cmd = get_command(cmd_node);
	if (!ft_strchr(cmd_node->name->input, '/'))
		cmd_node->name->input = find_cmd_abs_path(shell, cmd_node);
	pid = fork();
	if (error(pid == -1, PERROR_MSG, 1, shell))
		return (free_2d(cmd), 1);
	if (pid == 0)
	{
		execute_cmd_child(shell, cmd_node, cmd_node->name->input, cmd);
		close_fds(cmd_node->red_lst);
	}
	waitpid(pid, &(shell->ex_code), 0);
	if (WIFEXITED(shell->ex_code))
		set_status(shell, WEXITSTATUS(shell->ex_code));
	else
		set_status(shell, WTERMSIG(shell->ex_code));
	free_2d(cmd);
	return (0);
}

void	execute_cmd_node(t_shell *shell, t_ast_node *node)
{
	t_cmd	*cmd;

	cmd = node->node;
	if (cmd->in_fd == -2)
		cmd->in_fd = 0;
	if (cmd->out_fd == -2)
		cmd->out_fd = 1;
	if (cmd->red_lst && execute_redir(shell, cmd->red_lst))
	{
		set_status(shell, 1);
		return ;
	}
	if (!check_builtins(node->node))
		call_builtins(shell, node->node);
	else
		execute_cmd(shell, node);
	close_fds(cmd->red_lst);
	if (cmd->in_fd != STDIN_FILENO)
		close(cmd->in_fd);
	if (cmd->out_fd != STDOUT_FILENO)
		close(cmd->out_fd);
}

void	execute_subshell_node(t_shell *shell, t_ast_node *node)
{
	pid_t		pid;
	t_ast_node	*sub_node;

	pid = fork();
	if (error(pid == -1, PERROR_MSG, 1, shell))
		return ;
	sub_node = node->node;
	if (node->red_lst && execute_redir(shell, node->red_lst))
	{
		set_status(shell, 1);
		return ;
	}
	if (pid == 0)
	{
		if (sub_node->type == AST_SUBSHELL)
			execute_subshell_node(shell, sub_node);
		else
			execute(shell, sub_node);
		waitpid(pid, &(shell->ex_code), 0);
		if (WIFEXITED(shell->ex_code))
			exit(WEXITSTATUS(shell->ex_code));
		else
			exit(WTERMSIG(shell->ex_code));
	}
	subshell_wait(shell, node, &pid);
}
