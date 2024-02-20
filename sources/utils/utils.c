/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 18:28:18 by mnazarya          #+#    #+#             */
/*   Updated: 2024/02/20 16:03:28 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*join_with_symbol(char *s1, char *s2, char c)
{
	char	*str;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (0);
	str = ft_calloc(sizeof(*s1), (ft_strlen(s1) + ft_strlen(s2) + 2));
	error_exit(!str, "malloc", 12);
	i = 0;
	j = 0;
	while (s1[i])
		str[j++] = s1[i++];
	str[j++] = c;
	i = 0;
	while (s2[i])
		str[j++] = s2[i++];
	str[j] = '\0';
	return (str);
}

void	free_2d(char **str)
{
	int	i;

	i = -1;
	if (!str)
		return ;
	while (str && str[++i])
		free(str[i]);
	free(str);
}

void	quotes_to_unprint(char *str)
{
	if (!str)
		return ;
	while (*str)
	{
		if (*str == '\'')
			*str = 1;
		if (*str == '\"')
			*str = 2;
		str ++;
	}
}

int	set_status(t_shell *shell, int stat)
{
	char	*status;

	shell->ex_code = stat;
	status = ft_itoa(shell->ex_code);
	add_env_node(1, "?", status, shell);
	free(status);
	return (shell->ex_code);
}
