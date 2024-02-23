/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 11:02:40 by mnazarya          #+#    #+#             */
/*   Updated: 2024/01/28 07:39:08 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	is_env_name(char *s)
{
	if (!s || !*s)
		return (0);
	if (ft_isalpha(*s) || *s == '_')
	{
		while (*s)
		{
			if (!ft_isalnum(*s) && *s != '_')
				break ;
			s++;
		}
		if (!(*s))
			return (1);
	}
	return (0);
}

void	ft_append(char **str, char c)
{
	char	s[2];

	s[0] = c;
	s[1] = 0;
	if (str)
	{
		if (!(*str))
			*str = ft_strdup(s);
		else
			*str = ft_join_free(*str, s);
	}
}

char	*find_dollar(char *str, int flag)
{
	static int	f = 0;

	while (str && *str)
	{
		if (flag & F_SQUOTES && *str == SQUOTES && !f)
		{
			str++;
			while (*str && *str != SQUOTES)
				str++;
			str++;
		}
		else if (*str == DQUOTES)
		{
			if (f)
				f = 0;
			else
				f = 1;
			str++;
		}
		else if (*str == DOLLAR)
			return ((char *)str);
		else
			str++;
	}
	return (0);
}
