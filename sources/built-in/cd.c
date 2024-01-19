/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 07:21:57 by mnazarya          #+#    #+#             */
/*   Updated: 2024/01/18 16:27:09 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	default_cd(t_shell *shell, char **path)
{
	char	*home;

	home = find_env_param(shell, "HOME");
	if (!home)
	{
		ft_putstr_fd(ERR_CD, 2);
		return (set_status(shell, 1));
	}
	*path = home;
	return (0);
}

static int	set_pwd(t_shell *shell)
{
	char	*new_pwd;
	char	*current_pwd;

	if (find_env_param(shell, "PWD"))
		current_pwd = ft_strdup(find_env_param(shell, "PWD"));
	else
		current_pwd = ft_strdup("");
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		perror(ERR_MSG);
		free(current_pwd);
		return (1);
	}
	add_env_node(0, "PWD", new_pwd, shell);
	add_env_node(0, "OLDPWD", current_pwd, shell);
	free(current_pwd);
	free(new_pwd);
	return (0);
}

int	cd(t_shell *shell, t_cmd *cmd)
{
	char	*path;

	path = NULL;
	if (!cmd->args)
		default_cd(shell, &path);
	else
		path = cmd->args->input;
	if (chdir(path) != 0 && path)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		perror(path);
		set_status(shell, 1);
	}
	if (!path)
		free(path);
	return (set_pwd(shell));
}