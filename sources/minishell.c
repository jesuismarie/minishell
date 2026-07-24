/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <mnazarya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 14:39:35 by mnazarya          #+#    #+#             */
/*   Updated: 2026/07/13 11:09:13 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	g_stat = 0;

static void	minishell_init(int argc, char **argv, char **envp, t_shell *shell)
{
	(void)argv;
	if (argc != 1)
		exit(0);
	shell->env = NULL;
	printf("\033[1;34m ███▄ ▄███▓ ██▓ ███▄    █  ██▓  ██████  ██░ ██ ▓█████  ██▓     ██▓    \n\033[0m");
	printf("\033[1;34m▓██▒▀█▀ ██▒▓██▒ ██ ▀█   █ ▓██▒▒██    ▒ ▓██░ ██▒▓█   ▀ ▓██▒    ▓██▒    \n\033[0m");
	printf("\033[1;34m▓██    ▓██░▒██▒▓██  ▀█ ██▒▒██▒░ ▓██▄   ▒██▀▀██░▒███   ▒██░    ▒██░    \n\033[0m");
	printf("\033[1;34m▒██    ▒██ ░██░▓██▒  ▐▌██▒░██░  ▒   ██▒░▓█ ░██ ▒▓█  ▄ ▒██░    ▒██░    \n\033[0m");
	printf("\033[1;34m▒██▒   ░██▒░██░▒██░   ▓██░░██░▒██████▒▒░▓█▒░██▓░▒████▒░██████▒░██████▒\n\033[0m");
	printf("\033[1;34m▒██▒   ░██▒░██░▒██░   ▓██░░██░▒██████▒▒░▓█▒░██▓░▒████▒░██████▒░██████▒\n\033[0m");
	printf("\033[1;34m░ ▒░   ░  ░░▓  ░ ▒░   ▒ ▒ ░▓  ▒ ▒▓▒ ▒ ░ ▒ ░░▒░▒░░ ▒░ ░░ ▒░▓  ░░ ▒░▓  ░\n\033[0m");
	printf("\033[1;34m░  ░      ░ ▒ ░░ ░░   ░ ▒░ ▒ ░░ ░▒  ░ ░ ▒ ░▒░ ░ ░ ░  ░░ ░ ▒  ░░ ░ ▒  ░\n\033[0m");
	printf("\033[1;34m░      ░    ▒ ░   ░   ░ ░  ▒ ░░  ░  ░   ░  ░░ ░   ░     ░ ░     ░ ░   \n\033[0m");
	printf("\033[1;34m       ░    ░           ░  ░        ░   ░  ░  ░   ░  ░    ░  ░    ░  ░\n\033[0m");
	printf("\n\033[1;34m Authors: mnazarya, ahovakim\n\n\033[0m");
	get_env(shell, envp);
	shell->prev = ft_strdup("");
	shell->hist = ft_strjoin(get_env_param(shell, "HOME"), \
	"/.minishell_history");
	shell->all_fds[0] = -1;
	shell->all_fds[1] = -1;
	shell->all_fds[2] = -1;
}

static void	prompt_init(t_shell *shell)
{
	g_stat = 0;
	shell->err = 0;
	shell->line = 0;
	shell->flag = 0;
	shell->index = 3;
	shell->ex_code = 0;
	shell->err_msg = ft_strdup("");
	sig_init(shell);
	if (shell->all_fds[0] >= 3)
		close(shell->all_fds[0]);
	if (shell->all_fds[1] >= 3)
		close(shell->all_fds[1]);
	if (shell->all_fds[2] >= 3)
		close(shell->all_fds[2]);
	shell->line = readline(PS);
	shell->all_fds[0] = dup(STDIN_FILENO);
	shell->all_fds[1] = dup(STDOUT_FILENO);
	shell->all_fds[2] = dup(STDERR_FILENO);
	dup2(shell->all_fds[0], STDIN_FILENO);
	dup2(shell->all_fds[1], STDOUT_FILENO);
	dup2(shell->all_fds[2], STDERR_FILENO);
	eof_handler(shell);
	nl_handler(shell->line);
}

int	prompt_validation(t_shell *shell)
{
	shell->token_head = input_scanner(shell->line);
	check_brace(shell, &(shell->token_head));
	shell->ex_code = token_analyser(shell, shell->token_head);
	check_open_close(shell);
	if (g_stat < 0)
		search_heredoc(shell, shell->token_head);
	check_here_count(shell);
	shell_history(shell);
	free(shell->line);
	if (g_stat < 0 || g_stat == SIGINT || g_stat == 1)
	{
		if (g_stat == SIGINT)
			set_status(shell, SIGINT + 128);
		else if (g_stat == 1)
			set_status(shell, ft_atoi(get_env_param(shell, "?")));
		else
		{
			ft_putstr_fd(shell->err_msg, 2);
			set_status(shell, shell->ex_code);
		}
		free(shell->err_msg);
		token_free(&(shell->token_head));
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	t_token	*tok;

	minishell_init(argc, argv, envp, &shell);
	while (1)
	{
		prompt_init(&shell);
		if (!shell.line)
			continue ;
		if (prompt_validation(&shell))
		{
			close_all_fds(&shell);
			continue ;
		}
		tok = shell.token_head;
		shell.tree = line_parsing(&shell, &tok);
		if (g_stat != SIGINT)
			execute(&shell, shell.tree);
		while (wait(&(shell.ex_code)) > -1)
			;
		ex_code_wait(&shell);
		clean_each_prompt(&shell);
	}
	return (0);
}
