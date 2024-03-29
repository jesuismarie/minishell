/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 13:49:16 by mnazarya          #+#    #+#             */
/*   Updated: 2024/02/08 06:54:16 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	clean(t_shell *shell)
{
	t_env	*tmp;

	while (shell->env_lst)
	{
		tmp = shell->env_lst->next;
		free(shell->env_lst->var_name);
		free(shell->env_lst->var_value);
		free(shell->env_lst);
		shell->env_lst = tmp;
	}
	free_2d(shell->env);
	free(shell->hist);
	free(shell->prev);
	free(shell->err_msg);
}
