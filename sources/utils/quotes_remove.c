/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_remove.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 18:28:48 by mnazarya          #+#    #+#             */
/*   Updated: 2024/01/07 05:43:27 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	check_quotes_count(char *str)
{
	int		i;
	int		n;
	char	a;

	i = 0;
	n = 0;
	while (str[i])
	{
		if (str[i] == '\"' || str[i] == '\'')
		{
			a = str[i];
			i++;
			n += 2;
			while (str[i] != a)
				i++;
		}
		i++;
	}
	return (n);
}

char	*remove_quotes(char *str)
{
	int		i;
	int		j;
	char	a;
	char	*s;

	i = -1;
	j = -1;
	if (strlen(str) - check_quotes_count(str) == 0)
		return (str);
	s = malloc(strlen(str) - check_quotes_count(str) + 1);
	while (str[++i])
	{
		if (str[i] == '\"' || str[i] == '\'')
		{
			a = str[i];
			while (str[++i] && str[i] != a)
				s[++j] = str[i];
		}
		else
			s[++j] = str[i];
	}
	s[++j] = 0;
	free(str);
	return (s);
}
