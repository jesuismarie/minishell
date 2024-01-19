/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/11 13:16:28 by mnazarya          #+#    #+#             */
/*   Updated: 2024/01/14 11:34:30 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_token	*get_operator_token(char **s)
{
	t_token	*node;

	if (!s || !(*s))
		return (NULL);
	node = ft_calloc(sizeof(t_token), 1);
	error_exit(!node, "malloc", 12);
	node->cmd = ft_calloc(sizeof(t_input), 1);
	error_exit(!node->cmd, "malloc", 12);
	node->type = get_op_type(s);
	operator_input(node);
	node->cmd->flag = 0;
	node->cmd->next = NULL;
	node->cmd->prev = NULL;
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

t_token	*get_word_token(char **s)
{
	t_input	*cur;
	t_token	*node;

	node = ft_calloc(sizeof(t_token), 1);
	error_exit(!node, "malloc", 12);
	cur = get_word(s);
	if (cur->flag & F_DOLLAR || cur->flag & F_MUL_DOLLAR)
		node->type = ENV_PARAM;
	else
		node->type = WORD;
	node->cmd = cur;
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

t_token	*get_token(char **s)
{
	if (!(*s))
		return (NULL);
	while (**s && ft_iswhitespace(**s))
		(*s)++;
	if (**s && ft_strchr(OPERATORS, **s))
		return (get_operator_token(s));
	else if (**s)
		return (get_word_token(s));
	return (NULL);
}
