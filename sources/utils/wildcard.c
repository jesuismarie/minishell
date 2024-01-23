/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 03:30:09 by mnazarya          #+#    #+#             */
/*   Updated: 2024/01/23 00:33:19 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	search_wildcard(char *str)
{
	char	quote;

	while (*str)
	{
		if (*str == DQUOTES || *str == SQUOTES)
		{
			quote = *str;
			str++;
			while (*str && *str != quote)
				str++;
			if (*str)
				str++;
		}
		if (*str == WILDCARD)
			return (1);
		if (*str)
			str++;
	}
	return (0);
}

void	replace_wildcard(t_input **arg)
{
	t_input	*wdc;
	t_input	*tmp;
	t_input	*tmp_wdc;

	tmp = *arg;
	wdc = read_directory(tmp);
	if (!wdc)
		return ;
	if (tmp->prev)
		tmp->prev->next = wdc;
	wdc->prev = tmp->prev;
	if (tmp->next)
		tmp->next->prev = wdc;
	tmp_wdc = wdc;
	wdc = ft_last_arg(tmp_wdc);
	wdc->next = tmp->next;
	wdc = tmp_wdc;
	free(tmp->input);
	free(tmp);
	*arg = wdc;
}

int	wd_match(char *ptr, char *txt)
{
	while (*ptr)
	{
		if (*ptr == WILDCARD)
		{
			while (*ptr == WILDCARD)
				ptr++;
			if (*ptr == '\0')
				return (1);
			while (*txt)
			{
				if (wd_match(ptr, txt))
					return (1);
				txt++;
			}
			return (0);
		}
		else if (*ptr == *txt)
		{
			ptr++;
			txt++;
		}
		else
			return (0);
	}
	return (*ptr == *txt);
}

t_input	*read_directory(t_input *args)
{
	DIR				*dir;
	t_input			*wdc;
	struct dirent	*entry;

	wdc = NULL;
	dir = opendir(".");
	if (dir == NULL)
	{
		perror(PERROR_MSG);
		exit(1);
	}
	while (1)
	{
		entry = readdir(dir);
		if (entry == NULL)
			break ;
		if (ft_strchr(args->input, '.') || entry->d_name[0] != '.')
			if (wd_match(args->input, entry->d_name))
				ft_args_add(&wdc, ft_new_arg(entry->d_name));
	}
	closedir(dir);
	return (wdc);
}
