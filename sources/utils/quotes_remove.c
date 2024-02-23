/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_remove.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 18:28:48 by mnazarya          #+#    #+#             */
/*   Updated: 2024/02/19 23:37:45 by mnazarya         ###   ########.fr       */
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
			n++;
			while (str[i] && str[i] != a)
				i++;
			if (str[i] == a)
				n++;
		}
		if (str[i])
			i++;
	}
	return (n);
}

static void	quotes_backup(char *str, char *s, int i, int *j)
{
	if (str[i] == 1)
		s[++(*j)] = '\'';
	else if (str[i] == 2)
		s[++(*j)] = '\"';
	else
		s[++(*j)] = str[i];
}

char	*remove_quotes(char *str)
{
	int		i;
	int		j;
	char	a;
	char	*s;

	i = -1;
	j = -1;
	s = ft_calloc(ft_strlen(str) - check_quotes_count(str) + 1, sizeof(char));
	while (str[++i])
	{
		if (str[i] == '\"' || str[i] == '\'')
		{
			a = str[i];
			while (str[++i] && str[i] != a)
				quotes_backup(str, s, i, &j);
		}
		else
			quotes_backup(str, s, i, &j);
	}
	s[++j] = 0;
	free(str);
	return (s);
}
