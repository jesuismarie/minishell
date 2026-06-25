/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <mnazarya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 13:49:16 by mnazarya          #+#    #+#             */
/*   Updated: 2026/06/25 15:28:38 by mnazarya         ###   ########.fr       */
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
	if (shell->err_msg)
		free(shell->err_msg);
	if (shell->prev)
		free(shell->prev);
	if (shell->hist)
		free(shell->hist);
	if (shell->line && shell->token_head)
		token_free(&(shell->token_head));
	if (shell->line && shell->tree)
		free_ast(&(shell->tree));
	if (shell->all_fds[0] >= 3)
		close(shell->all_fds[0]);
	if (shell->all_fds[1] >= 3)
		close(shell->all_fds[1]);
	if (shell->all_fds[2] >= 3)
		close(shell->all_fds[2]);
}
