/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 16:49:22 by mnazarya          #+#    #+#             */
/*   Updated: 2023/11/29 20:49:39 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*initialize_name(char **envp, int i, int j)
{
	char	*res;
	int		k;

	k = 0;
	res = ft_calloc(sizeof(char), j + 1);
	error_exit(!res, "malloc", 12);
	while (k < j)
	{
		res[k] = envp[i][k];
		k++;
	}
	res[k] = '\0';
	return (res);
}

char	*initialize_value(char **envp, int i, int j)
{
	char	*res;
	int		k;

	k = 0;
	res = ft_calloc(sizeof(char), ft_strlen(envp[i]) - j + 1);
	while (envp[i][++j])
	{
		res[k] = envp[i][j];
		k++;
	}
	res[k] = '\0';
	return (res);
}

static t_env	*get_env_list(t_env *cur)
{
	cur->hidden = 0;
	cur->next = ft_calloc(sizeof(t_env), 1);
	error_exit(!cur->next, "malloc", 12);
	cur->next->next = NULL;
	cur->next->prev = cur;
	return (cur->next);
}

void	get_env(t_shell *shell, char **envp)
{
	t_env	*cur;
	int		i;
	int		j;

	i = -1;
	shell->env_lst = ft_calloc(sizeof(t_env), 1);
	error_exit(!shell->env_lst, "malloc", 12);
	cur = shell->env_lst;
	while (envp[++i])
	{
		j = -1;
		while (envp[i][++j])
		{
			if (envp[i][j] == '=')
			{
				cur->var_name = initialize_name(envp, i, j);
				cur->var_value = initialize_value(envp, i, j);
				break ;
			}
		}
		if (envp[i + 1])
			cur = get_env_list(cur);
	}
	add_hidden_values(shell);
}

char	**env_vars(t_shell *shell)
{
	int		i;
	t_env	*tmp;
	int		count;
	char	**env;

	i = 0;
	tmp = shell->env_lst;
	count = env_lenght(shell);
	env = malloc(sizeof(char *) * (count + 1));
	while (tmp)
	{
		if (tmp->hidden == 0)
		{
			env[i] = join_with_symbol(tmp->var_name, tmp->var_value, '=');
			i++;
		}
		tmp = tmp->next;
	}
	env[i] = 0;
	return (env);
}
