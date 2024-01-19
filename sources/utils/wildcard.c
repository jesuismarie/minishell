/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 03:30:09 by mnazarya          #+#    #+#             */
/*   Updated: 2024/01/19 13:09:50 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

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

int	wd_match(char *ptr, char *txt)
{
	while (*ptr)
	{
		if (*ptr == '*') {
			while (*ptr == '*')
				ptr++;
			if (*ptr == '\0')
				return (1);
			while (*txt) {
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
