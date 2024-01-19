/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 19:19:52 by mnazarya          #+#    #+#             */
/*   Updated: 2024/01/11 03:51:47 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static t_redir	*parse_helper(t_shell *shell, t_token **tok_lst)
{
	t_redir	*redir_lst;
	t_redir	*tmp;

	redir_lst = NULL;
	while ((*tok_lst) && ((*tok_lst)->type == HEREDOC \
	|| (*tok_lst)->type == APPEND || (*tok_lst)->type == FILE_IN \
	|| (*tok_lst)->type == FILE_OUT))
	{
		if (redir_lst)
		{
			redir_lst->next = new_redir_node(shell, tok_lst);
			redir_lst->next->next = NULL;
			redir_lst = redir_lst->next;
		}
		else
		{
			redir_lst = new_redir_node(shell, tok_lst);
			redir_lst->next = NULL;
			tmp = redir_lst;
		}
	}
	return (tmp);
}

t_ast_node	*parse_redir(t_shell *shell, t_token **tok_lst)
{
	t_ast_node	*redir;

	redir = NULL;
	if ((*tok_lst) && ((*tok_lst)->type == HEREDOC \
	|| (*tok_lst)->type == APPEND || (*tok_lst)->type == FILE_IN \
	|| (*tok_lst)->type == FILE_OUT))
	{
		redir = ft_calloc(sizeof(t_ast_node), 1);
		error_exit(!redir, "malloc", 12);
		redir->type = AST_REDIRECTION;
		redir->node = (void *)parse_helper(shell, tok_lst);
		redir->subshell_flag = 0;
	}
	return (redir);
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
