/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 19:48:21 by mnazarya          #+#    #+#             */
/*   Updated: 2024/01/23 00:21:34 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static char	*get_key(char *str)
{
	int		len;
	char	*key;
	char	*tmp;

	len = 1;
	tmp = NULL;
	key = NULL;
	while (*(str + len))
	{
		ft_append(&tmp, *(str + len));
		if (!ft_isalpha(*(str + len)) && *(str + len) != '_' \
		&& ((ft_isdigit(*(str + len)) && len == 1) \
		|| !ft_isdigit(*(str + len))))
			break ;
		ft_append(&key, *(str + len));
		len++;
	}
	if (ft_isdigit(*tmp) || !ft_strcmp(tmp, "?") || !ft_strcmp(tmp, "$"))
		key = tmp;
	else
		free(tmp);
	return (key);
}

static void	replace_key(t_shell *shell, char **expanded, char **str, char *dol)
{
	char	*key;
	char	*value;

	key = get_key(dol);
	if (key && (ft_isalpha(*key) || *key != '_' || ft_isdigit(*key)))
	{
		value = NULL;
		value = get_env_param(shell, key);
		*expanded = ft_join_free(*expanded, value);
		*str += ft_strlen(key) + 1;
		free(key);
		key = NULL;
	}
	else
	{
		ft_append(expanded, **str);
		(*str)++;
	}
}

char	*expand_vars(t_shell *shell, char *str, int flag)
{
	char	*s;
	char	*dol;
	char	*tmp;
	char	*expanded;

	s = str;
	expanded = NULL;
	while (*str)
	{
		dol = find_dollar(str, flag);
		if (!dol)
			break ;
		tmp = ft_substr(str, 0, dol - str);
		expanded = ft_join_free(expanded, tmp);
		free(tmp);
		str = dol;
		replace_key(shell, &expanded, &str, dol);
	}
	expanded = ft_join_free(expanded, str);
	free(s);
	return (expanded);
}
