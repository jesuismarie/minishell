/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 07:21:57 by mnazarya          #+#    #+#             */
/*   Updated: 2024/01/27 06:54:06 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	default_cd(t_shell *shell, char **path)
{
	char	*home;

	home = get_env_param(shell, "HOME");
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
	static int		current = 0;
	char			*new_pwd;
	char			*current_pwd;

	if (get_env_param(shell, "PWD"))
	{
		current_pwd = ft_strdup(get_env_param(shell, "PWD"));
		if (!get_env_flag(shell, "PWD"))
			current = 0;
	}
	else
	{
		current_pwd = ft_strdup("");
		current = 1;
	}
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		perror(ERR_MSG);
		free(current_pwd);
		return (1);
	}
	add_env_node(current, "PWD", new_pwd, shell);
	add_env_node(0, "OLDPWD", current_pwd, shell);
	return (free(current_pwd), free(new_pwd), 0);
}

int	cd(t_shell *shell, t_cmd *cmd)
{
	char	*path;

	path = NULL;
	if (!cmd->args || (cmd->args->flag & F_EXPANDED))
	{
		default_cd(shell, &path);
		if (path && chdir(path) != 0)
		{
			ft_putstr_fd("minishell: cd: ", 2);
			perror(path);
			set_status(shell, 1);
			return (1);
		}
	}
	else if (cmd->args && cmd->args->flag & F_DEL_QUOTES)
		return (0);
	else
		path = cmd->args->input;
	if (path && chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		perror(path);
		set_status(shell, 1);
	}
	return (set_pwd(shell));
}
