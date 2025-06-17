/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 12:20:55 by mnazarya          #+#    #+#             */
/*   Updated: 2024/02/20 16:39:28 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	args_count(t_input *args)
{
	int		n;

	n = 0;
	while (args)
	{
		args = args->next;
		n++;
	}
	return (n);
}

char	**get_command(t_cmd *cmd)
{
	int		i;
	t_input	*arg;
	char	**splt_cmd;

	i = 1;
	arg = cmd->args;
	splt_cmd = ft_calloc(args_count(arg) + 2, sizeof(char *));
	splt_cmd[0] = ft_strdup(cmd->name->input);
	while (arg && !(arg->flag & F_DEL_QUOTES))
	{
		splt_cmd[i] = ft_strdup(arg->input);
		i++;
		arg = arg->next;
	}
	splt_cmd[i] = 0;
	return (splt_cmd);
}

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
	if (cmd->name->flag & F_DEL_QUOTES)
		return (cmd->name->input);
	env = get_path(shell);
	if (!env)
		return (cmd->name->input);
	tmp = ft_strjoin("/", cmd->name->input);
	while (env[++i])
	{
		path = ft_strjoin(env[i], tmp);
		if (!access(path, F_OK))
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
