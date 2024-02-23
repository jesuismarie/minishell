/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 12:09:53 by mnazarya          #+#    #+#             */
/*   Updated: 2024/02/03 02:23:22 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	print_history(t_shell *shell, t_cmd	*cmd)
{
	int		n;
	char	*str;

	n = 1;
	if (cmd->args)
	{
		ft_putstr_fd(ERR_HIST, 2);
		set_status(shell, 1);
		return ;
	}
	shell->history_fd = open(shell->hist, O_RDONLY);
	while (1)
	{
		str = get_next_line(shell->history_fd);
		if (!str)
			break ;
		printf("   %d %s", n++, str);
		free(str);
		str = NULL;
	}
	close(shell->history_fd);
}

void	shell_history(t_shell *shell)
{
	if (shell->line == NULL || ft_strcmp(shell->line, "") == 0)
		return ;
	shell->history_fd = open(shell->hist, O_CREAT \
	| O_WRONLY | O_APPEND, 0644);
	if (ft_strcmp(shell->line, shell->prev) && shell->line[0] != ' ')
	{
		add_history(shell->line);
		if (shell->history_fd != -1 && g_stat != -3)
			ft_putendl_fd(shell->line, shell->history_fd);
	}
	free(shell->prev);
	shell->prev = ft_strdup(shell->line);
	close(shell->history_fd);
}
