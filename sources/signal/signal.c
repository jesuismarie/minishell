/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 09:57:01 by mnazarya          #+#    #+#             */
/*   Updated: 2024/02/03 04:09:39 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	quit_handler(void)
{
	return (0);
}

static void	handle_signal(int sig)
{
	if (sig == SIGINT)
	{
		g_stat = SIGINT;
		rl_replace_line("", 0);
		rl_done = 1;
	}
}

void	eof_handler(t_shell *shell)
{
	if (!shell->line)
	{
		write(1, "\033[A", 3);
		ft_putstr_fd(PS, 1);
		ft_putstr_fd("exit\n", 1);
		clean(shell);
		exit(shell->ex_code);
	}
}

void	sig_init(t_shell *shell)
{
	struct sigaction	sa;

	sa.sa_handler = handle_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (error(sigaction(SIGINT, &sa, NULL) == -1, \
	"Sigaction error ❌", 128 + SIGINT, shell))
		return ;
	if (error(sigaction(SIGQUIT, &sa, NULL) == -1, \
	"Sigaction error ❌", 128 + SIGQUIT, shell))
		return ;
	rl_catch_signals = 0;
	rl_event_hook = &quit_handler;
}
