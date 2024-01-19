/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 12:20:55 by mnazarya          #+#    #+#             */
/*   Updated: 2024/01/17 03:54:05 by mnazarya         ###   ########.fr       */
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
		path = ft_join_free(env[i], tmp);
		if (!access(path, X_OK))
		{
			free(tmp);
			free(cmd->name->input);
			return (path);
		}
		free(path);
	}
	free(tmp);
	return (cmd->name->input);
}

void	call_builtins(t_shell *shell, t_cmd *cmd)
{
	if (!ft_strcmp(cmd->name->input, "pwd"))
		pwd(shell);
	else if (!ft_strcmp(cmd->name->input, "env"))
		print_env(shell);
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
