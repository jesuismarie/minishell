/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 16:40:50 by mnazarya          #+#    #+#             */
/*   Updated: 2023/08/25 12:48:53 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_token	*last_token(t_token *tok_lst)
{
	while (tok_lst)
	{
		if (!tok_lst->next)
			return (tok_lst);
		tok_lst = tok_lst->next;
	}
	return (tok_lst);
}

void	token_add(t_token **tok_lst, t_token *token)
{
	t_token	*tmp;

	if (!*tok_lst)
		*tok_lst = token;
	else
	{
		tmp = *tok_lst;
		*tok_lst = last_token(*tok_lst);
		(*tok_lst)->next = token;
		if ((*tok_lst)->next)
			(*tok_lst)->next->prev = *tok_lst;
		*tok_lst = tmp;
	}
}
