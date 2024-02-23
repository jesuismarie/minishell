/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 06:38:36 by mnazarya          #+#    #+#             */
/*   Updated: 2024/02/22 11:08:05 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static char	*get_key(char *str)
{
	int		len;
	char	*key;
	char	*tmp;

	len = 1;
	tmp = NULL;
	key = NULL;
	while (*(str + len))
	{
		ft_append(&tmp, *(str + len));
		if (!ft_isalpha(*(str + len)) && *(str + len) != '_' \
		&& ((ft_isdigit(*(str + len)) && len == 1) \
		|| !ft_isdigit(*(str + len))))
			break ;
		ft_append(&key, *(str + len));
		len++;
	}
	if (tmp && (ft_isdigit(*tmp) || !ft_strcmp(tmp, "?") \
	|| !ft_strcmp(tmp, "$")))
		key = tmp;
	else
		free(tmp);
	return (key);
}

static void	replace_key(t_shell *shell, char **expanded, char **str, char *dol)
{
	char	*key;
	char	*value;

	key = get_key(dol);
	if (key && (ft_isalnum(*key) || *key == '_' || ft_strcmp(key, "$") \
	|| ft_strcmp(key, "?")))
	{
		value = NULL;
		value = get_env_param(shell, key);
		quotes_to_unprint(value);
		*expanded = ft_join_free(*expanded, value);
		*str += ft_strlen(key) + 1;
		free(key);
		key = NULL;
	}
	else
	{
		ft_append(expanded, **str);
		(*str)++;
	}
}

char	*expand_vars(t_shell *shell, char *str, int flag)
{
	char	*s;
	char	*dol;
	char	*tmp;
	char	*expanded;

	s = str;
	expanded = NULL;
	while (*str)
	{
		dol = find_dollar(str, flag);
		if (!dol)
			break ;
		tmp = ft_substr(str, 0, dol - str);
		expanded = ft_join_free(expanded, tmp);
		free(tmp);
		str = dol;
		replace_key(shell, &expanded, &str, dol);
	}
	expanded = ft_join_free(expanded, str);
	free(s);
	return (expanded);
}

static void	expand_args(t_shell *shell, t_cmd *cmd)
{
	t_input	**tmp;

	tmp = &cmd->args;
	while (*tmp)
	{
		if ((*tmp)->flag & (F_DOLLAR | F_MUL_DOLLAR))
		{
			(*tmp)->input = expand_vars(shell, (*tmp)->input, (*tmp)->flag);
			if ((*tmp)->input && !(*tmp)->input[0])
				(*tmp)->flag |= F_EXPANDED;
		}
		if (search_wildcard((*tmp)->input))
			replace_wildcard(tmp);
		(*tmp)->input = remove_quotes((*tmp)->input);
		if ((*tmp)->input && !(*tmp)->input[0])
			(*tmp)->flag |= F_DEL_QUOTES;
		tmp = &(*tmp)->next;
	}
}

void	expand(t_shell *shell, t_ast_node *node)
{
	t_cmd	*cmd;

	cmd = node->node;
	if (cmd->name && cmd->name->flag & (F_DOLLAR | F_MUL_DOLLAR))
		cmd->name->input = expand_vars(shell, \
		cmd->name->input, cmd->name->flag);
	if (cmd->name && cmd->name->flag & (F_SQUOTES | F_DQUOTES))
	{
		cmd->name->input = remove_quotes(cmd->name->input);
		if (cmd->name->input && !cmd->name->input[0])
			cmd->name->flag |= F_DEL_QUOTES;
	}
	if (cmd->args)
		expand_args(shell, cmd);
}
