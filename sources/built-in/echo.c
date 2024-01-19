/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 08:37:13 by mnazarya          #+#    #+#             */
/*   Updated: 2024/01/16 13:19:35 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	check_nl_flag(char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (0);
	if (str[i] == '-')
		i++;
	while (str[i] == 'n')
		i++;
	if (i == ft_strlen(str))
		return (1);
	return (0);
}

static char	*create_str(t_cmd *cmd)
{
	char	*str;
	t_input	*tmp;

	str = ft_strdup("");
	tmp = cmd->args;
	while (tmp && check_nl_flag(tmp->input))
		tmp = tmp->next;
	while (tmp)
	{
		str = ft_join_free(str, tmp->input);
		tmp = tmp->next;
		if (tmp)
			str = ft_join_free(str, " ");
	}
	return (str);
}

int	echo(t_shell *shell, t_cmd *cmd)
{
	int		f;
	char	*str;

	if (!cmd->args)
		ft_putstr_fd("\n", 1);
	else
	{
		f = check_nl_flag(cmd->args->input);
		str = create_str(cmd);
		ft_putstr_fd(str, 1);
		if (!f)
			ft_putstr_fd("\n", 1);
		free(str);
	}
	return (set_status(shell, 0));
}
