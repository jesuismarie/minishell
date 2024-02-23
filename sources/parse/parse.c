/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 21:28:10 by mnazarya          #+#    #+#             */
/*   Updated: 2023/12/27 11:49:23 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_ast_node	*line_parsing(t_shell *shell, t_token **tok_lst)
{
	t_ast_node	*tree;

	tree = NULL;
	if (!tok_lst)
		return (NULL);
	tree = parse_pipeline(shell, tok_lst);
	return (parse_logic_op(shell, tree, tok_lst));
}
