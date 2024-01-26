/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_node.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 07:01:38 by mnazarya          #+#    #+#             */
/*   Updated: 2024/01/26 06:00:26 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	add_env_node(int hidden, char *name, char *value, t_shell *shell)
{
	t_env	*tmp;

	tmp = shell->env_lst;
	while (tmp)
	{
		if (tmp && !ft_strcmp(name, tmp->var_name))
		{
			free(tmp->var_value);
			tmp->var_value = ft_strdup(value);
			tmp->hidden = hidden;
			init_env(shell);
			return ;
		}
		if (!tmp->next)
			break ;
		tmp = tmp->next;
	}
	tmp->next = ft_calloc(sizeof(t_env), 1);
	error_exit(!tmp->next, "malloc", 12);
	tmp->next->var_name = ft_strdup(name);
	tmp->next->var_value = ft_strdup(value);
	tmp->next->hidden = hidden;
	tmp->next->prev = tmp;
	init_env(shell);
}

void	del_env_node(char *var_name, t_shell *shell)
{
	t_env	*tmp;

	tmp = shell->env_lst;
	while (tmp)
	{
		if (!ft_strcmp(var_name, tmp->var_name))
		{
			free(tmp->var_name);
			free(tmp->var_value);
			if (tmp->next)
			{
				tmp->prev->next = tmp->next;
				tmp->next->prev = tmp->prev;
			}
			else
				tmp->prev->next = NULL;
			free(tmp);
			return ;
		}
		tmp = tmp->next;
	}
	init_env(shell);
}

char	*get_env_param(t_shell *shell, char *key)
{
	t_env	*tmp;

	tmp = shell->env_lst;
	while (tmp)
	{
		if (!ft_strcmp(key, tmp->var_name))
			return (tmp->var_value);
		tmp = tmp->next;
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
