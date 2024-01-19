/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 13:34:12 by mnazarya          #+#    #+#             */
/*   Updated: 2024/01/19 03:46:17 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	check_num(char *str)
{
	if (*str == '-')
		str++;
	while (*str && *str >= '0' && *str <= '9')
		str++;
	if (!*str)
		return (1);
	return (0);
}

static void	numeric_arg_err(t_shell *shell, char *num)
{
	ft_putstr_fd("exit\nminishell: exit: ", 1);
	ft_putstr_fd(num, 2);
	ft_putstr_fd(" numeric argument required\n", 2);
	set_status(shell, 2);
	exit(2);
}

uint64_t	ft_atoi_64(char *nptr)
{
	int			i;
	int			j;
	uint64_t	nb;

	if (!nptr)
		return (0);
	i = 0;
	nb = 0;
	j = 1;
	while (nptr[i] == '\n' || nptr[i] == ' ' || nptr[i] == '\t'
		|| nptr[i] == '\f' || nptr[i] == '\v' || nptr[i] == '\r')
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			j = -j;
		i++;
	}
	while (nptr[i] >= 48 && nptr[i] <= 57)
		nb = (nb * 10) + (nptr[i++] - 48);
	return (nb * j);
}

static int	do_exit(t_shell *shell, t_cmd *cmd)
{
	int			flag;
	char		*tmp;
	uint64_t	ex_code;

	ex_code = 0;
	flag = 0;
	tmp = cmd->args->input;
	if (*tmp == '-')
	{
		tmp++;
		flag = 1;
	}
	while (*tmp == '0')
		tmp++;
	if (ft_strlen(tmp) > 19 || (flag == 0 \
	&& ft_strcmp(tmp, "9223372036854775807") > 0) \
	|| (flag == 1 && ft_strcmp(tmp, "9223372036854775808") > 0))
		numeric_arg_err(shell, cmd->args->input);
	ex_code = ft_atoi_64(cmd->args->input) % 256;
	if ((int)ex_code < 0)
		ex_code -= 256;
	return (ex_code);
}

int	my_exit(t_shell *shell, t_cmd *cmd)
{
	if (!cmd->args)
	{
		ft_putstr_fd("exit\n", 1);
		set_status(shell, 0);
		exit(0);
	}
	else if (cmd->args && cmd->args->next)
	{
		ft_putstr_fd("exit\n", 1);
		ft_putstr_fd(ERR_EXIT, 2);
		return (set_status(shell, 2));
	}
	else if (!check_num(cmd->args->input))
		numeric_arg_err(shell, cmd->args->input);
	exit(do_exit(shell, cmd));
	return (0);
}
