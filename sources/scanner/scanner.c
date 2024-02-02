/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 15:17:48 by mnazarya          #+#    #+#             */
/*   Updated: 2023/08/04 15:17:48 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_token	*input_scanner(char *str)
{
	t_token	*tok_lst;
	t_token	*token;

	tok_lst = NULL;
	token = NULL;
	while (*str)
	{
		token = get_token(&str);
		token_add(&tok_lst, token);
	}
	return (tok_lst);
}

void	token_free(t_token **tok_lst)
{
	t_token	*tmp;

	while (*tok_lst)
	{
		tmp = (*tok_lst)->next;
		free((*tok_lst)->cmd->input);
		free((*tok_lst)->cmd);
		free(*tok_lst);
		*tok_lst = tmp;
	}
}

static int	error_analyser(t_shell *shell)
{
	set_err(shell, ERR_AND);
	g_stat = -2;
	return (2);
}

int	token_analyser(t_shell *shell, t_token *tok_lst)
{
	t_token	*tmp;

	tmp = tok_lst;
	while (tmp && tmp->type != ERROR && !shell->ex_code)
	{
		if (tmp->type == PIPE_OP || tmp->type == OR_OP || tmp->type == AND_OP)
			shell->ex_code = operator_analyser(shell, &tmp);
		else if (tmp->type == WORD)
			shell->ex_code = word_analyser(shell, &tmp);
		else if (tmp->type == BRACE_OPEN)
			shell->ex_code = open_brace_analyser(shell, &tmp);
		else if (tmp->type == HEREDOC || tmp->type == APPEND \
		|| tmp->type == FILE_IN || tmp->type == FILE_OUT)
			shell->ex_code = redirections_analyser(shell, &tmp);
		else if (tmp->type == BRACE_CLOSE)
			shell->ex_code = close_brace_analyser(shell, &tmp);
		else if (tmp->type == ENV_PARAM)
			shell->ex_code = env_param_analizer(shell, &tmp);
		else
			tmp = tmp->next;
	}
	if (tmp && tmp->type == ERROR)
		shell->ex_code = error_analyser(shell);
	return (set_status(shell, shell->ex_code));
}
