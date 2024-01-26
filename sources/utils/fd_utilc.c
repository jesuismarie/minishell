/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_utilc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 07:16:17 by mnazarya          #+#    #+#             */
/*   Updated: 2024/01/09 14:15:06 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	fds_init(t_shell *shell)
{
	int	i;

	i = -1;
	while (shell->all_fds[++i])
		shell->all_fds[i] = -1;
}

void	close_fds(t_shell *shell)
{
	int	i;

	i = -1;
	while (shell->all_fds[++i])
		close(shell->all_fds[i]);
}
