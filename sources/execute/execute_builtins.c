/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 19:25:11 by mnazarya          #+#    #+#             */
/*   Updated: 2024/02/20 11:58:52 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	check_builtins(t_cmd *cmd)
{
	if (!cmd->name)
		return (1);
	if (!ft_strcmp(cmd->name->input, "pwd"))
		return (0);
	else if (!ft_strcmp(cmd->name->input, "env"))
		return (0);
	else if (!ft_strcmp(cmd->name->input, "echo"))
		return (0);
	else if (!ft_strcmp(cmd->name->input, "unset"))
		return (0);
	else if (!ft_strcmp(cmd->name->input, "export"))
		return (0);
	else if (!ft_strcmp(cmd->name->input, "cd"))
		return (0);
	else if (!ft_strcmp(cmd->name->input, "exit"))
		return (0);
	else if (!ft_strcmp(cmd->name->input, "history"))
		return (0);
	return (1);
}

void	call_builtins(t_shell *shell, t_cmd *cmd)
{
	if (!ft_strcmp(cmd->name->input, "pwd"))
		pwd(shell);
	else if (!ft_strcmp(cmd->name->input, "env"))
		print_env(shell, cmd);
	else if (!ft_strcmp(cmd->name->input, "echo"))
		echo(shell, cmd);
	else if (!ft_strcmp(cmd->name->input, "unset"))
		unset(shell, cmd);
	else if (!ft_strcmp(cmd->name->input, "export"))
		export(shell, cmd);
	else if (!ft_strcmp(cmd->name->input, "cd"))
		cd(shell, cmd);
	else if (!ft_strcmp(cmd->name->input, "exit"))
		my_exit(shell, cmd);
	else if (!ft_strcmp(cmd->name->input, "history"))
		print_history(shell, cmd);
}

int	handle_builtins(t_shell *shell, t_ast_node *node)
{
	t_cmd	*cmd;

	cmd = node->node;
	if (dup2(cmd->in_fd, STDIN_FILENO) == -1 \
	|| dup2(cmd->out_fd, STDOUT_FILENO) == -1)
	{
		perror(PERROR_MSG);
		exit(1);
	}
	if (cmd->red_lst && execute_redir(shell, cmd->red_lst))
		return (1);
	call_builtins(shell, cmd);
	dup2(shell->all_fds[0], STDIN_FILENO);
	dup2(shell->all_fds[1], STDOUT_FILENO);
	return (0);
}
