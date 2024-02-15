/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 19:25:11 by mnazarya          #+#    #+#             */
/*   Updated: 2024/02/15 19:34:14 by mnazarya         ###   ########.fr       */
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

static void	builtins_helper(t_shell *shell, t_cmd *cmd)
{
	int	ecode;

	if (dup2(cmd->in_fd, 0) == -1 || dup2(cmd->out_fd, 1) == -1)
	{
		perror(PERROR_MSG);
		exit(1);
	}
	if (cmd->red_lst && execute_redir(shell, cmd->red_lst))
		exit(1);
	close_all_fds(shell);
	call_builtins(shell, cmd);
	ecode = ft_atoi(get_env_param(shell, "?"));
	exit(ecode);
}

int	handle_builtins(t_shell *shell, t_ast_node *node)
{
	pid_t	pid;
	t_cmd	*cmd;

	cmd = node->node;
	if (ft_strcmp(cmd->name->input, "cd") \
	|| ft_strcmp(cmd->name->input, "exit") \
	|| ft_strcmp(cmd->name->input, "unset") \
	|| ft_strcmp(cmd->name->input, "export"))
		call_builtins(shell, cmd);
	else
	{
		pid = fork();
		if (error(pid == -1, PERROR_MSG, 1, shell))
			return (1);
		if (pid == 0)
			builtins_helper(shell, cmd);
		waitpid(pid, &(shell->ex_code), 0);
		ex_code_wait(shell);
	}
	return (0);
}
