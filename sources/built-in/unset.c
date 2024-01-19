/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 04:55:12 by mnazarya          #+#    #+#             */
/*   Updated: 2024/01/16 13:20:59 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	unset(t_shell *shell, t_cmd *cmd)
{
	t_input	*tmp;

	tmp = cmd->args;
	while (tmp)
	{
		if (is_env_name(tmp->input))
		{
			del_env_node(tmp->input, shell);
			shell->ex_code = 0;
		}
		else if (!ft_strcmp(tmp->input, "?") \
		|| !ft_strcmp(tmp->input, "$") \
		|| !ft_strcmp(tmp->input, "0"))
			continue ;
		else
			builtin_err(shell, ERR_UNSET, tmp->input);
		tmp = tmp->next;
	}
	return (set_status(shell, shell->ex_code)); //poxeeel
}
