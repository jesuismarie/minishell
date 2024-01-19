/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 16:53:59 by mnazarya          #+#    #+#             */
/*   Updated: 2024/01/16 13:18:44 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	print_delim_warning(t_input *word)
{
	ft_putstr_fd("minishell: warning: ", 1);
	ft_putstr_fd(ERR_LIM, 1);
	ft_putstr_fd(word->input, 1);
	ft_putstr_fd("\')", 1);
}

static void	fake_here_child(t_input *word)
{
	char	*line;

	while (1)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
		line = readline("> ");
		if (!line)
		{
			print_delim_warning(word);
			break ;
		}
		if (word && word->input && !ft_strcmp(line, word->input))
		{
			if (line)
				free (line);
			break ;
		}
		free(line);
	}
	exit(0);
}

void	fake_heredoc(t_shell *shell, t_token *lim)
{
	t_input	*word;
	int		pid;

	set_attr(0);
	pid = fork();
	if (error(pid < 0, PERROR_MSG, 1, shell))
		return ;
	if (pid == 0)
	{
		word = lim->cmd;
		fake_here_child(word);
	}
	waitpid(pid, &(shell->ex_code), 0);
	if (WIFEXITED(shell->ex_code))
		set_status(shell, WEXITSTATUS(shell->ex_code));
	else if (WIFSIGNALED(shell->ex_code))
	{
		write(1, "\n", 1);
		set_status(shell, WTERMSIG(shell->ex_code) + 128);
	}
	set_attr(1);
}

void	heredoc(t_shell *shell, t_input *word, t_redir *node)
{
	char	*line;

	(void)shell;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline("> ");
		if (!line)
			print_delim_warning(word);
		if (!line || (word && word->input && !ft_strcmp(line, word->input)))
			break ;
		// if (!(word->flag & (SQUOTES | DQUOTES)) && ft_strchr(line, '$'))
		// 	line = expand_param(shell, line, F_DOLLAR);
			// expand_param(shell, &line, F_DOLLAR);
		ft_putstr_fd(line, node->out_fd);
		free(line);
	}
	close(node->in_fd);
	close(node->out_fd);
	exit(0);
}
