/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator_scan.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 18:45:45 by mnazarya          #+#    #+#             */
/*   Updated: 2023/12/04 12:38:23 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	check_redir(char **s, t_token_type type, char c)
{
	(*s)++;
	if (**s && **s == c)
	{
		(*s)++;
		return (type);
	}
	else
		return (type + 2);
}

static int	check_pipe(char **s, t_token_type type, char c)
{
	(*s)++;
	if (**s && **s == c)
	{
		(*s)++;
		return (type + 2);
	}
	else
		return (type);
}

void	operator_input(t_token *node)
{
	if (node->type == HEREDOC)
		node->cmd->input = ft_strdup("<<");
	else if (node->type == APPEND)
		node->cmd->input = ft_strdup(">>");
	else if (node->type == OR_OP)
		node->cmd->input = ft_strdup("||");
	else if (node->type == AND_OP)
		node->cmd->input = ft_strdup("&&");
	else if (node->type == FILE_IN)
		node->cmd->input = ft_strdup("<");
	else if (node->type == FILE_OUT)
		node->cmd->input = ft_strdup(">");
	else if (node->type == PIPE_OP)
		node->cmd->input = ft_strdup("|");
	else if (node->type == BRACE_OPEN)
		node->cmd->input = ft_strdup("(");
	else if (node->type == BRACE_CLOSE)
		node->cmd->input = ft_strdup(")");
	else if (node->type == ERROR)
		node->cmd->input = ft_strdup("&");
}

static void	spp(char **s)
{
	(*s)++;
}

int	get_op_type(char **s)
{
	if (**s == LBRACE)
		return (spp(s), BRACE_OPEN);
	else if (**s == RBRACE)
		return (spp(s), BRACE_CLOSE);
	else if (**s == PIPE)
		return (check_pipe(s, PIPE_OP, PIPE));
	else if (**s == '&')
	{
		(*s)++;
		if (**s && **s == '&')
		{
			(*s)++;
			return (AND_OP);
		}
	}
	else if (**s == LESS)
		return (check_redir(s, HEREDOC, LESS));
	else if (**s == GREAT)
		return (check_redir(s, APPEND, GREAT));
	return (ERROR);
}
