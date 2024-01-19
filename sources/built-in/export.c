/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 06:46:35 by mnazarya          #+#    #+#             */
/*   Updated: 2024/01/16 07:24:21 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	default_export(t_shell *shell)
{
	t_env	*tmp;

	tmp = shell->env_lst;
	while (tmp)
	{
		if (tmp->hidden != 1)
		{
			printf("declare -x %s", tmp->var_name);
			if (tmp->hidden == 0)
				printf("=\"%s\"", tmp->var_value);
			printf("\n");
		}
		tmp = tmp->next;
	}
	return (0);
}

int	export(t_shell *shell, t_cmd *cmd)
{
	char	*name;
	char	*tmp;
	t_input	*arg_tmp;

	if (!cmd->args)
		return (default_export(shell));
	arg_tmp = cmd->args;
	while (arg_tmp)
	{
		tmp = ft_strchr(arg_tmp->input, '=');
		name = ft_substr(arg_tmp->input, 0, \
		ft_strlen(arg_tmp->input) - ft_strlen(tmp));
		if (!is_env_name(name))
			builtin_err(shell, ERR_EXPORT, arg_tmp->input);
		else
		{
			if (!tmp)
				add_env_node(-1, arg_tmp->input, NULL, shell);
			else if (arg_tmp->flag & F_ASSIGNMENT)
				add_env_node(0, name, ++tmp, shell);
		}
		free(name);
		arg_tmp = arg_tmp->next;
	}
	return (1);
}
