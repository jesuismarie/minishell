/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 18:28:18 by mnazarya          #+#    #+#             */
/*   Updated: 2024/01/20 09:33:43 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*join_with_symbol(char *s1, char *s2, char c)
{
	char	*str;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (0);
	str = ft_calloc(sizeof(*s1), (ft_strlen(s1) + ft_strlen(s2) + 2));
	error_exit(!str, "malloc", 12);
	i = 0;
	j = 0;
	while (s1[i])
		str[j++] = s1[i++];
	str[j++] = c;
	i = 0;
	while (s2[i])
		str[j++] = s2[i++];
	str[j] = '\0';
	return (str);
}

char	*get_env_param(t_shell *shell, char *key)
{
	t_env	*tmp;

	tmp = shell->env_lst;
	if (key)
	{
		while (tmp)
		{
			if (!ft_strcmp(key, tmp->var_name))
				return (tmp->var_value);
			tmp = tmp->next;
		}
	}
	return (NULL);
}

char	*get_env_name(t_shell *shell, char *key)
{
	t_env	*tmp;

	tmp = shell->env_lst;
	while (tmp)
	{
		if (!ft_strcmp(key, tmp->var_name))
			return (tmp->var_name);
		tmp = tmp->next;
	}
	return (NULL);
}

int	set_status(t_shell *shell, int stat)
{
	char	*status;

	shell->ex_code = stat;
	status = ft_itoa(shell->ex_code);
	add_env_node(1, "?", status, shell);
	free(status);
	return (shell->ex_code);
}
