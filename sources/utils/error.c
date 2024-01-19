/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/11 17:10:54 by mnazarya          #+#    #+#             */
/*   Updated: 2024/01/16 13:22:06 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	error(int cond, char *str, int ecode, t_shell *shell)
{
	if (cond)
	{
		perror(str);
		set_status(shell, ecode);
		g_stat = -1;
		return (1);
	}
	return (0);
}

void	error_exit(int cond, char *str, int ecode)
{
	if (cond)
	{
		perror(str);
		exit(ecode);
	}
}

void	set_error_stat(int stat, t_token **lst)
{
	g_stat = stat;
	(*lst)->type = ERROR;
}

void	set_err(t_shell *shell, char *str)
{
	if (shell->err_msg && ft_strcmp(shell->err_msg, ""))
		return ;
	free(shell->err_msg);
	shell->err_msg = ft_strdup(str);
}

void	builtin_err(t_shell *shell, char *err, char *input)
{
	ft_putstr_fd(err, 2);
	ft_putchar_fd('`', 2);
	ft_putstr_fd(input, 2);
	ft_putstr_fd("\': not a valid identifier\n", 2);
	shell->ex_code = 1;
}
