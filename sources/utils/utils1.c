/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 11:02:40 by mnazarya          #+#    #+#             */
/*   Updated: 2024/01/17 03:41:26 by mnazarya         ###   ########.fr       */
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
	char	*s;

	s = ft_calloc(sizeof(char), 2);
	s[0] = c;
	s[1] = '\0';
	if (str)
	{
		if (!(*str))
		{
			*str = ft_strdup(s);
			free(s);
		}
		else
		{
			*str = ft_join_free(*str, s);
			free(s);
		}
	}
}

char	*find_dollar(char *str, int flag)
{
	while (str && *str)
	{
		if (flag & F_SQUOTES && *str == SQUOTES)
		{
			str++;
			while (*str != SQUOTES)
				str++;
		}
		if (*str == DOLLAR)
			return ((char *)str);
		str++;
	}
	return (0);
}
