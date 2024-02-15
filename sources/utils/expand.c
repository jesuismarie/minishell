/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 06:38:36 by mnazarya          #+#    #+#             */
/*   Updated: 2024/02/15 14:15:30 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	expand_args(t_shell *shell, t_cmd *cmd)
{
	t_input	**tmp;

	tmp = &cmd->args;
	while (*tmp)
	{
		if ((*tmp)->flag & (F_DOLLAR | F_MUL_DOLLAR))
		{
			(*tmp)->input = expand_vars(shell, (*tmp)->input, (*tmp)->flag);
			if ((*tmp)->input && !(*tmp)->input[0])
				(*tmp)->flag |= F_EXPANDED;
		}
		if (search_wildcard((*tmp)->input))
			replace_wildcard(tmp);
		if ((*tmp) && (*tmp)->flag & (F_SQUOTES | F_DQUOTES))
		{
			(*tmp)->input = remove_quotes((*tmp)->input);
			if ((*tmp)->input && !(*tmp)->input[0])
				(*tmp)->flag |= F_DEL_QUOTES;
		}
		tmp = &(*tmp)->next;
	}
}

void	expand(t_shell *shell, t_ast_node *node)
{
	t_cmd	*cmd;

	cmd = node->node;
	if (cmd->name && cmd->name->flag & (F_DOLLAR | F_MUL_DOLLAR))
		cmd->name->input = expand_vars(shell, \
		cmd->name->input, cmd->name->flag);
	if (cmd->name && cmd->name->flag & (F_SQUOTES | F_DQUOTES))
		cmd->name->input = remove_quotes(cmd->name->input);
	if (cmd->args)
		expand_args(shell, cmd);
}
