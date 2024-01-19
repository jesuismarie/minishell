/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/09 03:31:24 by mnazarya          #+#    #+#             */
/*   Updated: 2024/01/14 12:16:29 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	word_count(t_token *tok_lst)
{
	int		n;

	n = 0;
	while (tok_lst && (tok_lst->type == WORD || tok_lst->type == ENV_PARAM))
	{
		n++;
		tok_lst = tok_lst->next;
	}
	return (n);
}

t_ast_node	*parse_simple_cmd(t_shell *shell, t_token **tok_lst)
{
	int		n;
	t_cmd	*cmd;

	cmd = ft_calloc(sizeof(t_cmd), 1);
	error_exit(!cmd, "malloc", 12);
	n = word_count(*tok_lst);
	cmd->in_fd = -2;
	cmd->out_fd = -2;
	cmd->name = ft_calloc(sizeof(t_input), 1);
	error_exit(!cmd->name, "malloc", 12);
	cmd->name->flag = (*tok_lst)->cmd->flag;
	cmd->name->input = ft_strdup((*tok_lst)->cmd->input);
	if (n < 2)
		cmd->args = NULL;
	else
		(*tok_lst) = (*tok_lst)->next;
	return (new_cmd_node(shell, cmd, tok_lst, n));
}

static void	parse_cmd_helper(t_ast_node **node, t_ast_node **tmp)
{
	if (tmp && *tmp)
	{
		(*node)->prev = (*tmp);
		(*tmp)->next = (*node);
	}
	else
		(*node)->prev = NULL;
}

t_ast_node	*parse_cmd_line(t_shell *shell, t_token **tok_lst)
{
	t_ast_node	*node;
	t_ast_node	*tmp;

	tmp = NULL;
	node = parse_redir(shell, tok_lst);
	if (node)
	{
		tmp = node;
		node = node->next;
	}
	if (*tok_lst && (*tok_lst)->type == BRACE_OPEN)
	{
		node = parse_subshell(shell, tok_lst);
		parse_cmd_helper(&node, &tmp);
	}
	else if (*tok_lst && (*tok_lst)->type != AND_OP \
	&& (*tok_lst)->type != OR_OP && (*tok_lst)->type != PIPE_OP \
	&& (*tok_lst)->type != BRACE_CLOSE)
	{
		node = parse_simple_cmd(shell, tok_lst);
		parse_cmd_helper(&node, &tmp);
	}
	if (tmp)
		node = tmp;
	return (node);
}

t_ast_node	*parse_subshell(t_shell *shell, t_token **tok_lst)
{
	t_ast_node	*node;

	node = NULL;
	*tok_lst = (*tok_lst)->next;
	node = line_parsing(shell, tok_lst);
	if ((*tok_lst)->type == BRACE_CLOSE)
		*tok_lst = (*tok_lst)->next;
	node->subshell_flag = 1;
	node->next = parse_redir(shell, tok_lst);
	return (node);
}
