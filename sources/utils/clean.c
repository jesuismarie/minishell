/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <mnazarya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 13:49:16 by mnazarya          #+#    #+#             */
/*   Updated: 2026/07/25 18:28:22 by mnazarya         ###   ########.fr       */
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
	if (shell->line)
		free(shell->line);
	close_all_fds(shell);
}

void	clean_each_prompt(t_shell *shell)
{
	close_all_fds(shell);
	if (shell->token_head)
	{
		token_free(&(shell->token_head));
		shell->token_head = NULL;
	}
	if (shell->tree)
	{
		free_ast(&(shell->tree));
		shell->tree = NULL;
	}
	if (shell->err_msg)
	{
		free(shell->err_msg);
		shell->err_msg = NULL;
	}
}
