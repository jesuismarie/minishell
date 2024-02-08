/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 12:20:55 by mnazarya          #+#    #+#             */
/*   Updated: 2024/02/08 06:55:47 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	**get_path(t_shell *shell)
{
	t_env	*envp;

	envp = shell->env_lst;
	while (envp)
	{
		if (!ft_strcmp(envp->var_name, "PATH"))
			return (ft_split(envp->var_value, ':'));
		envp = envp->next;
	}
	return (NULL);
}

char	*find_cmd_abs_path(t_shell *shell, t_cmd *cmd)
{
	int		i;
	char	*tmp;
	char	*path;
	char	**env;

	i = -1;
	env = get_path(shell);
	tmp = ft_strjoin("/", cmd->name->input);
	while (env[++i])
	{
		path = ft_strjoin(env[i], tmp);
		if (!access(path, X_OK))
		{
			free(tmp);
			free(cmd->name->input);
			free_2d(env);
			return (path);
		}
		free(path);
	}
	free(tmp);
	free_2d(env);
	return (cmd->name->input);
}

char	**get_command(t_cmd *cmd)
{
	t_input	*arg;
	char	*tmp;
	char	*command;
	char	**splt_cmd;

	arg = cmd->args;
	if (arg)
	{
		command = join_with_symbol(cmd->name->input, arg->input, ' ');
		arg = arg->next;
	}
	else
		command = ft_strdup(cmd->name->input);
	while (arg)
	{
		tmp = join_with_symbol(command, arg->input, ' ');
		free(command);
		command = tmp;
		arg = arg->next;
	}
	splt_cmd = ft_split(command, ' ');
	free(command);
	return (splt_cmd);
}

int	check_builtins(t_cmd *cmd)
{
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
