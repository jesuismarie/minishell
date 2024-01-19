/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 01:30:54 by mnazarya          #+#    #+#             */
/*   Updated: 2024/01/16 13:22:37 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	set_attr(int mode)
{
	static struct termios	ts;

	if (mode == 0)
		tcgetattr(STDIN_FILENO, &ts);
	if (mode == 1)
		tcsetattr(STDIN_FILENO, TCSANOW, &ts);
}

void	search_heredoc(t_shell *shell, t_token *tok_lst)
{
	t_token	*tmp;

	tmp = tok_lst;
	while (tmp && tmp->type != ERROR)
	{
		if (tmp->type == HEREDOC)
		{
			fake_heredoc(shell, tmp->next);
			if (g_stat == 2)
				g_stat = -2;
		}
		tmp = tmp->next;
	}
}

void	hd_wait(t_shell *shell, int *pid, t_redir **node)
{
	waitpid(*pid, &(shell->ex_code), 0);
	if (WIFEXITED(shell->ex_code))
		set_status(shell, WEXITSTATUS(shell->ex_code));
	else if (WIFSIGNALED(shell->ex_code))
	{
		write(1, "\n", 1);
		set_status(shell, WTERMSIG(shell->ex_code) + 128);
		(*node)->in_fd = -1;
	}
}
