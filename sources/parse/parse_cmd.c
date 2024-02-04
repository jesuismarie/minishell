/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/09 03:31:24 by mnazarya          #+#    #+#             */
/*   Updated: 2024/02/04 14:21:32 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	word_count(t_token *tok_lst)
{
	int		n;

	n = 0;
	while (tok_lst && (tok_lst->type == WORD || tok_lst->type == ENV_PARAM \
	|| tok_lst->type == FILE_OUT || tok_lst->type == FILE_IN \
	|| tok_lst->type == APPEND || tok_lst->type == HEREDOC))
	{
		if (tok_lst->type == FILE_OUT || tok_lst->type == FILE_IN \
		|| tok_lst->type == APPEND || tok_lst->type == HEREDOC)
			while (tok_lst && (tok_lst->type == FILE_OUT \
			|| tok_lst->type == FILE_IN || tok_lst->type == APPEND \
			|| tok_lst->type == HEREDOC))
				tok_lst = tok_lst->next->next;
		else
		{
			n++;
			tok_lst = tok_lst->next;
		}
	}
	return (n);
}

t_ast_node	*parse_cmd(t_shell *shell, t_token **tok_lst, t_redir **red_lst)
{
	t_cmd	*cmd;

	cmd = ft_calloc(sizeof(t_cmd), 1);
	error_exit(!cmd, "malloc", 12);
	cmd->n = word_count(*tok_lst);
	cmd->in_fd = -2;
	cmd->out_fd = -2;
	cmd->name = ft_calloc(sizeof(t_input), 1);
	error_exit(!cmd->name, "malloc", 12);
	cmd->name->flag = (*tok_lst)->cmd->flag;
	cmd->name->input = ft_strdup((*tok_lst)->cmd->input);
	if (cmd->n < 2)
		cmd->args = NULL;
	else
		(*tok_lst) = (*tok_lst)->next;
	return (new_cmd_node(shell, cmd, tok_lst, red_lst));
}

static t_ast_node	*parse_cmd_helper(t_ast_node *cmd, t_redir *red_lst)
{
	t_cmd	*tmp;

	if (cmd && red_lst)
	{
		tmp = cmd->node;
		tmp->red_lst = red_lst;
	}
	else if (!cmd && red_lst)
	{
		cmd = ft_calloc(sizeof(t_ast_node), 1);
		error_exit(!cmd, "malloc", 12);
		cmd->type = AST_COMMAND;
		tmp = ft_calloc(sizeof(t_cmd), 1);
		tmp->name = NULL;
		tmp->args = NULL;
		tmp->in_fd = -2;
		tmp->out_fd = -2;
		tmp->n = 0;
		tmp->red_lst = red_lst;
		cmd->node = tmp;
	}
	return (cmd);
}

t_ast_node	*parse_cmd_line(t_shell *shell, t_token **tok_lst)
{
	t_ast_node	*cmd;
	t_redir		*red_lst;

	cmd = NULL;
	red_lst = NULL;
	if (*tok_lst && (*tok_lst)->type == BRACE_OPEN)
		cmd = parse_subshell(shell, tok_lst);
	else if (*tok_lst && (*tok_lst)->type != AND_OP \
	&& (*tok_lst)->type != OR_OP && (*tok_lst)->type != PIPE_OP \
	&& (*tok_lst)->type != BRACE_CLOSE)
	{
		red_lst = parse_redir(shell, tok_lst, &red_lst);
		if (*tok_lst && (*tok_lst)->type != AND_OP \
		&& (*tok_lst)->type != OR_OP && (*tok_lst)->type != PIPE_OP)
			cmd = parse_cmd(shell, tok_lst, &red_lst);
	}
	cmd = parse_cmd_helper(cmd, red_lst);
	return (cmd);
}

t_ast_node	*parse_subshell(t_shell *shell, t_token **tok_lst)
{
	t_redir		*red_lst;
	t_ast_node	*node;
	t_ast_node	*child;

	node = NULL;
	red_lst = NULL;
	node = ft_calloc(sizeof(t_ast_node), 1);
	error_exit(!node, "malloc", 12);
	*tok_lst = (*tok_lst)->next;
	child = line_parsing(shell, tok_lst);
	if ((*tok_lst)->type == BRACE_CLOSE)
		*tok_lst = (*tok_lst)->next;
	node->type = AST_SUBSHELL;
	node->red_lst = parse_redir(shell, tok_lst, &red_lst);
	node->node = child;
	return (node);
}
