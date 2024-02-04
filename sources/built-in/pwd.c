/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 09:41:57 by mnazarya          #+#    #+#             */
/*   Updated: 2024/01/28 07:55:49 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	pwd(t_shell *shell)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (pwd)
	{
		printf("%s\n", pwd);
		free(pwd);
		set_status(shell, 0);
		return ;
	}
	else if (!pwd && get_env_param(shell, "PWD"))
	{
		printf("%s\n", get_env_param(shell, "PWD"));
		set_status(shell, 0);
		return ;
	}
	perror(PERROR_MSG);
	set_status(shell, 1);
}
