/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_analyser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 11:06:04 by mnazarya          #+#    #+#             */
/*   Updated: 2024/01/31 07:11:11 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	operator_analyser(t_shell *shell, t_token **lst)
{
	char	*msg;

	if (!(*lst)->prev)
	{
		msg = ft_strjoin(ERR_MSG, (*lst)->cmd->input);
		msg = ft_join_free(msg, "'\n");
		set_error_stat(-2, lst);
		return (set_err(shell, msg), free(msg), 2);
	}
	else if (!(*lst)->next || ((*lst)->next && ((*lst)->next->type == PIPE_OP \
	|| (*lst)->next->type == OR_OP || (*lst)->next->type == AND_OP \
	|| (*lst)->next->type == BRACE_CLOSE)))
	{
		if ((*lst)->next)
			msg = ft_strjoin(ERR_MSG, (*lst)->next->cmd->input);
		else
			msg = ft_strjoin(ERR_MSG, (*lst)->cmd->input);
		msg = ft_join_free(msg, "\'\n");
		set_error_stat(-2, lst);
		return (set_err(shell, msg), free(msg), 2);
	}
	else
		*lst = (*lst)->next;
	return (0);
}

int	word_analyser(t_shell *shell, t_token **lst)
{
	char	*msg;

	if ((*lst)->next && (*lst)->next->next && (*lst)->next->type == BRACE_OPEN)
	{
		set_error_stat(-2, &(*lst)->next->next);
		msg = ft_strjoin(ERR_MSG, (*lst)->next->next->cmd->input);
		msg = ft_join_free(msg, "\'\n");
		return (set_err(shell, msg), free(msg), 2);
	}
	else
		*lst = (*lst)->next;
	return (0);
}

int	redirections_analyser(t_shell *shell, t_token **lst)
{
	char	*msg;

	if (!(*lst)->next)
	{
		set_error_stat(-3, lst);
		return (set_err(shell, ERR_NL), 2);
	}
	else if ((*lst)->next && ((*lst)->next->type != ENV_PARAM \
	&& (*lst)->next->type != WORD))
	{
		msg = ft_strjoin(ERR_MSG, (*lst)->next->cmd->input);
		msg = ft_join_free(msg, "\'\n");
		set_error_stat(-3, lst);
		return (set_err(shell, msg), free(msg), 2);
	}
	else if ((*lst)->next->next && (*lst)->next->next->type == BRACE_OPEN)
	{
		set_error_stat(-2, &(*lst)->next->next);
		set_err(shell, ERR_OP_B);
		return (2);
	}
	else
		*lst = (*lst)->next;
	return (0);
}

int	env_param_analizer(t_shell *shell, t_token **lst)
{
	if (!ft_strcmp((*lst)->cmd->input, "$") && (*lst)->next \
	&& (*lst)->next->type == BRACE_OPEN)
	{
		set_error_stat(-2, &(*lst)->next);
		set_err(shell, ERR_OP_B);
		return (2);
	}
	else if (!ft_strcmp((*lst)->cmd->input, "$"))
	{
		(*lst)->type = WORD;
		(*lst)->cmd->flag ^= F_DOLLAR;
	}
	*lst = (*lst)->next;
	return (0);
}
