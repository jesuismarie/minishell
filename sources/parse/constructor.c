/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 22:10:22 by mnazarya          #+#    #+#             */
/*   Updated: 2024/02/01 13:34:38 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_redir	*new_redir_node(t_shell *shell, t_token **tok_lst)
{
	t_redir		*node;

	node = ft_calloc(sizeof(t_redir), 1);
	error_exit(!node, "malloc", 12);
	node->type = (*tok_lst)->type;
	*tok_lst = (*tok_lst)->next;
	node->filename = new_word_node(tok_lst);
	if (node->type == HEREDOC)
		parse_heredoc(shell, &node);
	return (node);
}

t_input	*new_word_node(t_token **tok_lst)
{
	t_input		*word;

	word = ft_calloc(sizeof(t_input), 1);
	error_exit(!word, "malloc", 12);
	if ((*tok_lst) && (*tok_lst)->cmd && (*tok_lst)->cmd->input)
		word->input = ft_strdup((*tok_lst)->cmd->input);
	else
		word->input = ft_strdup("");
	word->input = remove_quotes(word->input);
	word->flag = (*tok_lst)->cmd->flag;
	word->next = NULL;
	if (*tok_lst)
		*tok_lst = (*tok_lst)->next;
	return (word);
}

void	add_cmd_node(t_token **tok_lst, t_cmd **cmd)
{
	t_input	*tmp;

	tmp = (*cmd)->args;
	if (!(*cmd)->args)
	{
		(*cmd)->args = ft_calloc(sizeof(t_input), 1);
		error_exit(!(*cmd)->args, "malloc", 12);
		(*cmd)->args->flag = (*tok_lst)->cmd->flag;
		(*cmd)->args->input = ft_strdup((*tok_lst)->cmd->input);
		(*cmd)->args->next = NULL;
		(*cmd)->args->prev = NULL;
	}
	else
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = ft_calloc(sizeof(t_input), 1);
		error_exit(!tmp, "malloc", 12);
		tmp->next->flag = (*tok_lst)->cmd->flag;
		tmp->next->input = ft_strdup((*tok_lst)->cmd->input);
		tmp->next->next = NULL;
		tmp->next->prev = tmp;
	}
	*tok_lst = (*tok_lst)->next;
}

t_ast_node	*new_cmd_node(t_shell *shell, t_cmd *cmd, t_token **tok, \
t_redir **red)
{
	t_ast_node	*node;

	node = ft_calloc(sizeof(t_ast_node), 1);
	if (cmd->n == 1)
		*tok = (*tok)->next;
	while (*tok && ((*tok)->type == WORD || (*tok)->type == ENV_PARAM \
	|| (*tok)->type == FILE_OUT || (*tok)->type == FILE_IN \
	|| (*tok)->type == APPEND || (*tok)->type == HEREDOC))
	{
		if ((*tok)->type == FILE_OUT || (*tok)->type == FILE_IN \
		|| (*tok)->type == APPEND || (*tok)->type == HEREDOC)
			*red = parse_redir(shell, tok, red);
		else
			add_cmd_node(tok, &cmd);
	}
	node->node = (void *)cmd;
	node->red_lst = NULL;
	node->type = AST_COMMAND;
	return (node);
}
