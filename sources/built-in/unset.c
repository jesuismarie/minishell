/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 04:55:12 by mnazarya          #+#    #+#             */
/*   Updated: 2024/02/19 15:38:16 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	unset(t_shell *shell, t_cmd *cmd)
{
	t_input	*tmp;

	tmp = cmd->args;
	while (tmp)
	{
		if (!check_name(tmp->input))
			builtin_err(shell, ERR_UNSET, tmp->input);
		if (ft_isalpha(tmp->input[0]) || tmp->input [0] == '_')
		{
			del_env_node(tmp->input, shell);
			shell->ex_code = 0;
		}
		else if (check_args(cmd))
		{
			tmp = tmp->next;
			continue ;
		}
		tmp = tmp->next;
	}
	return (set_status(shell, shell->ex_code));
}
