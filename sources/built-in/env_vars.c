/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 03:03:53 by mnazarya          #+#    #+#             */
/*   Updated: 2024/02/01 07:22:19 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	chech_shlvl(t_shell *shell)
{
	int64_t	lvl;
	char	*shlvl;

	if (!get_env_param(shell, "SHLVL"))
		shlvl = "0";
	else
		shlvl = get_env_param(shell, "SHLVL");
	lvl = ft_atoi_64(shlvl);
	if (lvl >= 999)
	{
		ft_putstr_fd("minishell: warning: shell level ", 2);
		ft_putnbr_fd(lvl + 1, 2);
		ft_putstr_fd(" too high, resetting to 1\n", 2);
		lvl = 1;
	}
	else if (lvl < 0)
		lvl = 0;
	else
		lvl += 1;
	shlvl = ft_itoa(lvl);
	add_env_node(0, "SHLVL", shlvl, shell);
	free(shlvl);
}
