/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brace_analyser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 06:14:18 by mnazarya          #+#    #+#             */
/*   Updated: 2024/02/01 08:11:52 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	analyser_helper(t_shell *shell, t_token **lst)
{
	while ((*lst)->next && ((*lst)->next->type == HEREDOC \
	|| (*lst)->next->type == APPEND || (*lst)->next->type == FILE_IN \
	|| (*lst)->next->type == FILE_OUT))
	{
		*lst = (*lst)->next;
		if ((*lst)->next->type != WORD \
		&& (*lst)->next->type != ENV_PARAM)
		{
			set_error_stat(-2, &(*lst)->next->next);
			return (set_err(shell, ERR_NL), 2);
		}
		*lst = (*lst)->next;
	}
	return (0);
}

int	close_brace_analyser(t_shell *shell, t_token **lst)
{
	char	*msg;

	if ((*lst)->next && ((*lst)->next->type == WORD \
	|| (*lst)->next->type == ENV_PARAM || (*lst)->next->type == BRACE_OPEN))
	{
		set_error_stat(-2, &(*lst));
		msg = ft_strjoin(ERR_MSG, (*lst)->next->cmd->input);
		msg = ft_join_free(msg, "\'\n");
		return (set_err(shell, msg), free(msg), 2);
	}
	shell->ex_code = analyser_helper(shell, lst);
	if ((*lst)->next && (*lst)->next->type != OR_OP \
	&& (*lst)->next->type != BRACE_CLOSE && (*lst)->next->type != AND_OP \
	&& (*lst)->next->type != PIPE_OP)
	{
		set_error_stat(-2, &(*lst));
		msg = ft_strjoin(ERR_MSG, (*lst)->next->cmd->input);
		msg = ft_join_free(msg, "\'\n");
		return (set_err(shell, msg), free(msg), 2);
	}
	else
		*lst = (*lst)->next;
	return (0);
}

static char	*set_brace_err(t_token **lst)
{
	char	*msg;

	msg = ft_strjoin(ERR_MSG, (*lst)->cmd->input);
	msg = ft_join_free(msg, "\'\n");
	set_error_stat(-2, lst);
	return (msg);
}

int	open_brace_analyser(t_shell *shell, t_token **lst)
{
	char	*msg;

	if ((*lst)->next && ((*lst)->next->type == HEREDOC \
	|| (*lst)->next->type == APPEND || (*lst)->next->type == FILE_IN \
	|| (*lst)->next->type == FILE_OUT))
	{
		if ((*lst)->next->next->type != WORD \
		&& (*lst)->next->next->type != ENV_PARAM)
		{
			set_error_stat(-2, &(*lst)->next->next);
			return (set_err(shell, ERR_CL_B), 2);
		}
		else
			*lst = (*lst)->next;
	}
	else if ((*lst)->next && ((*lst)->next->type == BRACE_CLOSE \
	|| (*lst)->next->type == PIPE_OP || (*lst)->next->type == OR_OP \
	|| (*lst)->next->type == AND_OP))
	{
		msg = set_brace_err(&(*lst)->next);
		return (set_err(shell, msg), free(msg), 2);
	}
	else
		*lst = (*lst)->next;
	return (0);
}
