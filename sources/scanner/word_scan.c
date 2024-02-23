/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_scan.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/11 13:15:22 by mnazarya          #+#    #+#             */
/*   Updated: 2024/01/23 13:28:33 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	check_quotes(t_input *current, int *i)
{
	if (current->input[*i] && current->input[*i] == SQUOTES)
	{
		current->flag |= F_SQUOTES;
		(*i)++;
		while (current->input[*i] && current->input[*i] != SQUOTES)
			(*i)++;
	}
	else if (current->input[*i] && current->input[*i] == DQUOTES)
	{
		current->flag |= F_DQUOTES;
		(*i)++;
		while (current->input[*i] && current->input[*i] != DQUOTES)
		{
			if (current->input[*i] && current->input[*i] == DOLLAR)
				current->flag |= F_MUL_DOLLAR;
			(*i)++;
		}
	}
}

static int	check_flags(t_input *current)
{
	int	i;

	i = 0;
	if (!current->input)
		return (0);
	current->flag = 0;
	while (current->input[i])
	{
		check_quotes(current, &i);
		if (current->input[i] && current->input[i] == DOLLAR)
			current->flag |= F_DOLLAR;
		if (current->input[i])
			i++;
	}
	if (ft_strchr(current->input, '='))
		current->flag |= F_ASSIGNMENT;
	return (current->flag);
}

t_input	*get_word(char **s)
{
	t_input	*cur;
	int		n;

	n = 0;
	cur = ft_calloc(1, sizeof(t_input));
	error_exit(!cur, "malloc", 12);
	while (*(*s + n) && *(*s + n) != '\t' \
		&& *(*s + n) != ' ' && !ft_strchr(OPERATORS, *(*s + n)))
	{
		quote_check(*s, &n);
		if (*(*s + n))
			n++;
	}
	cur->input = ft_substr(*s, 0, n);
	cur->flag = check_flags(cur);
	cur->next = NULL;
	cur->prev = NULL;
	*s += n;
	return (cur);
}
