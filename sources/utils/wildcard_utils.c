/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 12:19:27 by mnazarya          #+#    #+#             */
/*   Updated: 2024/01/20 06:58:53 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_input	*ft_new_arg(char *name)
{
	t_input	*arg;

	if (!name)
		return (0);
	arg = (t_input *)malloc(sizeof(t_input));
	if (!arg)
		return (0);
	arg->input = ft_strdup(name);
	arg->flag = 0;
	arg->next = NULL;
	arg->prev = NULL;
	return (arg);
}

t_input	*ft_last_arg(t_input *args)
{
	while (args)
	{
		if (!args->next)
			return (args);
		args = args->next;
	}
	return (args);
}

void	ft_args_add(t_input **args, t_input *new)
{
	t_input	*n;

	if (!args)
		return ;
	if (!*args)
		*args = new;
	else
	{
		n = ft_last_arg(*args);
		n->next = new;
		new->prev = n;
	}
}
