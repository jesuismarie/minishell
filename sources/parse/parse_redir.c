/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 19:19:52 by mnazarya          #+#    #+#             */
/*   Updated: 2024/02/01 13:27:42 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_redir	*parse_redir(t_shell *shell, t_token **tok, t_redir **redir)
{
	t_redir	*tmp;

	tmp = NULL;
	if (*redir)
		tmp = *redir;
	while ((*tok) && ((*tok)->type == HEREDOC \
	|| (*tok)->type == APPEND || (*tok)->type == FILE_IN \
	|| (*tok)->type == FILE_OUT))
	{
		if (*redir)
		{
			while ((*redir) && (*redir)->next)
				*redir = (*redir)->next;
			(*redir)->next = new_redir_node(shell, tok);
			(*redir)->next->next = NULL;
			*redir = (*redir)->next;
		}
		else
		{
			*redir = new_redir_node(shell, tok);
			(*redir)->next = NULL;
			tmp = *redir;
		}
	}
	return (tmp);
}

void	parse_heredoc(t_shell *shell, t_redir **node)
{
	int	pid;
	int	fds[2];

	set_attr(0);
	if (error(pipe(fds) == -1, PERROR_MSG, 1, shell))
	{
		(*node)->in_fd = -1;
		return ;
	}
	(*node)->in_fd = fds[0];
	(*node)->out_fd = fds[1];
	pid = fork();
	if (error(pid < 0, PERROR_MSG, 1, shell))
	{
		(*node)->in_fd = -1;
		return ;
	}
	if (pid == 0)
		heredoc(shell, (*node)->filename, *node);
	hd_wait(shell, &pid, node);
	set_attr(1);
}
