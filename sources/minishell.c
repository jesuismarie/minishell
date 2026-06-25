/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <mnazarya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 14:39:35 by mnazarya          #+#    #+#             */
/*   Updated: 2026/06/25 14:25:44 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	g_stat = 0;

static void	minishell_init(int argc, char **argv, char **envp, t_shell *shell)
{
	(void)argv;
	if (argc != 1)
		exit(0);
	shell->env = NULL;
	printf("\033[1;34m ███▄ ▄███▓ ██▓ ███▄    █  ██▓  ██████  ██░ ██ ▓█████  ██▓     ██▓    \n\033[0m");
	printf("\033[1;34m▓██▒▀█▀ ██▒▓██▒ ██ ▀█   █ ▓██▒▒██    ▒ ▓██░ ██▒▓█   ▀ ▓██▒    ▓██▒    \n\033[0m");
	printf("\033[1;34m▓██    ▓██░▒██▒▓██  ▀█ ██▒▒██▒░ ▓██▄   ▒██▀▀██░▒███   ▒██░    ▒██░    \n\033[0m");
	printf("\033[1;34m▒██    ▒██ ░██░▓██▒  ▐▌██▒░██░  ▒   ██▒░▓█ ░██ ▒▓█  ▄ ▒██░    ▒██░    \n\033[0m");
	printf("\033[1;34m▒██▒   ░██▒░██░▒██░   ▓██░░██░▒██████▒▒░▓█▒░██▓░▒████▒░██████▒░██████▒\n\033[0m");
	printf("\033[1;34m▒██▒   ░██▒░██░▒██░   ▓██░░██░▒██████▒▒░▓█▒░██▓░▒████▒░██████▒░██████▒\n\033[0m");
	printf("\033[1;34m░ ▒░   ░  ░░▓  ░ ▒░   ▒ ▒ ░▓  ▒ ▒▓▒ ▒ ░ ▒ ░░▒░▒░░ ▒░ ░░ ▒░▓  ░░ ▒░▓  ░\n\033[0m");
	printf("\033[1;34m░  ░      ░ ▒ ░░ ░░   ░ ▒░ ▒ ░░ ░▒  ░ ░ ▒ ░▒░ ░ ░ ░  ░░ ░ ▒  ░░ ░ ▒  ░\n\033[0m");
	printf("\033[1;34m░      ░    ▒ ░   ░   ░ ░  ▒ ░░  ░  ░   ░  ░░ ░   ░     ░ ░     ░ ░   \n\033[0m");
	printf("\033[1;34m       ░    ░           ░  ░        ░   ░  ░  ░   ░  ░    ░  ░    ░  ░\n\033[0m");
	printf("\n\033[1;34m Authors: mnazarya, ahovakim\n\n\033[0m");
	get_env(shell, envp);
	shell->prev = ft_strdup("");
	shell->hist = ft_strjoin(get_env_param(shell, "HOME"), \
	"/.minishell_history");
	shell->all_fds[0] = -1;
	shell->all_fds[1] = -1;
	shell->all_fds[2] = -1;
}

static void	prompt_init(t_shell *shell)
{
	g_stat = 0;
	shell->err = 0;
	shell->flag = 0;
	shell->index = 3;
	shell->ex_code = 0;
	shell->err_msg = ft_strdup("");
	sig_init(shell);
	if (shell->all_fds[0] >= 3)
		close(shell->all_fds[0]);
	if (shell->all_fds[1] >= 3)
		close(shell->all_fds[1]);
	if (shell->all_fds[2] >= 3)
		close(shell->all_fds[2]);
	shell->line = readline(PS);
	shell->all_fds[0] = dup(STDIN_FILENO);
	shell->all_fds[1] = dup(STDOUT_FILENO);
	shell->all_fds[2] = dup(STDERR_FILENO);
	dup2(shell->all_fds[0], STDIN_FILENO);
	dup2(shell->all_fds[1], STDOUT_FILENO);
	dup2(shell->all_fds[2], STDERR_FILENO);
	eof_handler(shell);
	nl_handler(shell->line);
}

int	prompt_validation(t_shell *shell)
{
	shell->token_head = input_scanner(shell->line);
	check_brace(shell, &(shell->token_head));
	shell->ex_code = token_analyser(shell, shell->token_head);
	check_open_close(shell);
	if (g_stat < 0)
		search_heredoc(shell, shell->token_head);
	check_here_count(shell);
	shell_history(shell);
	free(shell->line);
	if (g_stat < 0 || g_stat == SIGINT || g_stat == 1)
	{
		if (g_stat == SIGINT)
			set_status(shell, SIGINT + 128);
		else if (g_stat == 1)
			set_status(shell, ft_atoi(get_env_param(shell, "?")));
		else
		{
			ft_putstr_fd(shell->err_msg, 2);
			set_status(shell, shell->ex_code);
		}
		free(shell->err_msg);
		token_free(&(shell->token_head));
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	t_token	*tok;

	minishell_init(argc, argv, envp, &shell);
	while (1)
	{
		prompt_init(&shell);
		if (!shell.line)
			continue ;
		if (prompt_validation(&shell))
			continue ;
		tok = shell.token_head;
		shell.tree = line_parsing(&shell, &tok);
		if (g_stat != SIGINT)
			execute(&shell, shell.tree);
		while (wait(&(shell.ex_code)) > -1)
			;
		ex_code_wait(&shell);
		close_all_fds(&shell);
		token_free(&(shell.token_head));
		free_ast(&(shell.tree));
		free(shell.err_msg);
	}
	return (0);
}

// void	print_tok_lst(t_token *lst)
// {
// 	printf("\n----TOKEN LIST----\n");
// 	while (lst)
// 	{
// 		printf ("token type: %d, token flag: %d, token input: %s\n", 
// 		lst->type, lst->cmd->flag, lst->cmd->input);
// 		lst = lst->next;
// 	}
// }

// void	print_ast(t_ast_node *node, int n, int flag)
// {
// 	int			i;
// 	t_operator	*op;
// 	t_redir		*red;
// 	t_pipe		*pipe;
// 	t_cmd		*cmd;
// 	t_input		*tmp;

// 	if (!node)
// 		return ;
// 	i = -1;
// 	while (++i < n - 1)
// 		printf("	");
// 	if (node->type == AST_LOGICAL_OP)
// 	{
// 		op = (t_operator *)node->node;
// 		if (op->type == 6)
// 			printf("&&\n");
// 		else
// 			printf("||\n");
// 		print_ast(op->left, n + 1, 0);
// 		print_ast(op->right, n + 1, 0);
// 	}
// 	else if (node->type == AST_SUBSHELL)
// 	{
// 		printf("()");
// 		print_ast(node->node, n + 1, 1);
// 		if (node->red_lst)
// 		{
// 			red = node->red_lst;
// 			printf("subshell redir-> ");
// 			while (red)
// 			{
// 				if (red->type == 0)
// 					printf("<< ");
// 				else if (red->type == 1)
// 					printf(">> ");
// 				else if (red->type == 2)
// 					printf("< ");
// 				else
// 					printf("> ");
// 				printf("%s(%d), ", red->filename->input,
// 				red->filename->flag);
// 				red = red->next;
// 			}
// 		}
// 		flag = 0;
// 		printf("\n");
// 	}
// 	else if (node->type == AST_PIPE)
// 	{
// 		pipe = (t_pipe *)node->node;
// 		printf("|\n");
// 		print_ast(pipe->left, n + 1, 0);
// 		print_ast(pipe->right, n + 1, 0);
// 	}
// 	else if (node->type == AST_COMMAND)
// 	{
// 		cmd = (t_cmd *)node->node;
// 		if (cmd->name)
// 			printf("%s(%d), ", cmd->name->input, cmd->name->flag);
// 		tmp = cmd->args;
// 		while (tmp)
// 		{
// 			printf("%s(%d), ", tmp->input, tmp->flag);
// 			tmp = tmp->next;
// 		}
// 		if (cmd->red_lst)
// 		{
// 			printf("\n");
// 			red = cmd->red_lst;
// 			while (red)
// 			{
// 				if (red->type == 0)
// 					printf("<< ");
// 				else if (red->type == 1)
// 					printf(">> ");
// 				else if (red->type == 2)
// 					printf("< ");
// 				else
// 					printf("> ");
// 				printf("%s(%d), ", red->filename->input,
// 				red->filename->flag);
// 				red = red->next;
// 			}
// 		}
// 		if (!flag)
// 			printf("\n");
// 	}
// }
