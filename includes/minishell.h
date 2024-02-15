/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 00:23:21 by mnazarya          #+#    #+#             */
/*   Updated: 2024/02/15 20:57:43 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <libft.h>
# include <fcntl.h>
# include <stdio.h>
# include <errno.h>
# include <limits.h>
# include <signal.h>
# include <define.h>
# include <stdint.h>
# include <string.h>
# include <dirent.h>
# include <termios.h>
# include <sys/wait.h>
# include <get_next_line.h>
# include <readline/history.h>
# include <readline/readline.h>

extern int	g_stat;

/*----------------------------------------------------------------------------*/
/*----------------------------- LEXICAL ANALIZER -----------------------------*/
/*----------------------------------------------------------------------------*/
void		quote_check(char *line, int *i);
void		check_open_close(t_shell *shell);
void		check_brace(t_shell *shell, t_token **tok_lst);
void		check_here_count(t_shell *shell);
void		clear_stack(t_stack **brace);
void		operator_input(t_token *node);
int			get_op_type(char **s);
t_token		*get_operator_token(char **s);
t_input		*get_word(char **s);
t_token		*get_word_token(char **s);
t_token		*input_scanner(char *str);
t_token		*get_token(char **s);
void		token_add(t_token **tok_lst, t_token *token);
int			operator_analyser(t_shell *shell, t_token **lst);
int			word_analyser(t_shell *shell, t_token **lst);
int			open_brace_analyser(t_shell *shell, t_token **lst);
int			close_brace_analyser(t_shell *shell, t_token **lst);
int			redirections_analyser(t_shell *shell, t_token **lst);
int			env_param_analizer(t_shell *shell, t_token **lst);
int			token_analyser(t_shell *shell, t_token *tok);
void		token_free(t_token **tok_lst);

/*----------------------------------------------------------------------------*/
/*---------------------------------- PARSER ----------------------------------*/
/*----------------------------------------------------------------------------*/
t_input		*new_word_node(t_token **tok_lst);
t_redir		*new_redir_node(t_shell *shell, t_token **tok_lst);
t_ast_node	*new_cmd_node(t_shell *shell, t_cmd *cmd, t_token **tok, \
			t_redir **red);
void		add_cmd_node(t_token **tok_lst, t_cmd **cmd);
t_ast_node	*line_parsing(t_shell *shell, t_token **tok_lst);
t_ast_node	*parse_pipeline(t_shell *shell, t_token **tok_lst);
t_ast_node	*parse_logic_op(t_shell *shell, t_ast_node *left, t_token **scan);
t_ast_node	*parse_pipe(t_shell *shell, t_ast_node *left, t_token **tok_lst);
t_ast_node	*parse_cmd_line(t_shell *shell, t_token **tok_lst);
t_ast_node	*parse_subshell(t_shell *shell, t_token **tok_lst);
t_redir		*parse_redir(t_shell *shell, t_token **tok_lst, t_redir **red_lst);
t_input		*parse_filename(t_token **tok_lst);
t_ast_node	*cmd_helper(t_ast_node **cmd, t_ast_node **r1, t_ast_node **r2);
t_ast_node	*parse_cmd(t_shell *shell, t_token **tok_lst, t_redir **red_lst);
void		parse_heredoc(t_shell *shell, t_redir **node);
void		free_ast(t_ast_node **node);

/*----------------------------------------------------------------------------*/
/*--------------------------------- BUILT_IN ---------------------------------*/
/*----------------------------------------------------------------------------*/
void		pwd(t_shell *shell);
int			check_args(t_cmd *cmd);
int			export(t_shell *shell, t_cmd *cmd);
int			echo(t_shell *shell, t_cmd *cmd);
int			unset(t_shell *shell, t_cmd *cmd);
int			cd(t_shell *shell, t_cmd *cmd);
int64_t		ft_atoi_64(char *nptr);
int			my_exit(t_shell *shell, t_cmd *cmd);
char		*get_pid(void);
char		*initialize_name(char **envp, int i, int j);
char		*initialize_value(char **envp, int i, int j);
int			add_hidden_values(t_shell *shell);
void		check_shlvl(t_shell *shell);
void		get_env(t_shell *shell, char **envp);
void		add_env_node(int hidden, char *name, char *value, t_shell *shell);
void		del_env_node(char *var_name, t_shell *shell);
char		*get_env_param(t_shell *shell, char *key);
char		*get_env_name(t_shell *shell, char *key);
int			get_env_flag(t_shell *shell, char *key);
int			env_lenght(t_shell *shell);
char		**env_vars(t_shell *shell);
void		init_env(t_shell *shell);
void		print_env(t_shell *shell, t_cmd *cmd);
void		print_history(t_shell *shell, t_cmd *cmd);

/*----------------------------------------------------------------------------*/
/*--------------------------------- HISTORY ----------------------------------*/
/*----------------------------------------------------------------------------*/
void		shell_history(t_shell *shell);

/*----------------------------------------------------------------------------*/
/*---------------------------------- SIGNAL ----------------------------------*/
/*----------------------------------------------------------------------------*/
void		sig_init(t_shell *shell);
void		nl_handler(char *line);
void		eof_handler(t_shell *shell);

/*----------------------------------------------------------------------------*/
/*--------------------------------- EXECUTE ----------------------------------*/
/*----------------------------------------------------------------------------*/
char		**get_path(t_shell *shell);
char		*find_cmd_abs_path(t_shell *shell, t_cmd *cmd);
char		**get_command(t_cmd *cmd);
int			check_builtins(t_cmd *cmd);
void		cmd_print_err(t_cmd *node);
void		call_builtins(t_shell *shell, t_cmd *cmd);
int			execute(t_shell *shell, t_ast_node *node);
void		execute_log_op(t_shell *shell, t_ast_node *node);
void		fds_helper(t_shell *shell, t_ast_node *node, int in, int out);
void		set_subshell_fds(t_shell *shell, t_ast_node *sub, int in, int out);
void		execute_pipeline(t_shell *shell, t_ast_node *node);
void		execute_subshell_node(t_shell *shell, t_ast_node *node);
void		execute_cmd_node(t_shell *shell, t_ast_node *node);
int			handle_builtins(t_shell *shell, t_ast_node *node);
int			execute_redir(t_shell *shell, t_redir *red_lst);
void		ex_code_wait(t_shell *shell);
void		subshell_exit(t_shell *shell, int *pid);
void		close_fds(t_redir *red_lst);
void		close_all_fds(t_shell *shell);

/*----------------------------------------------------------------------------*/
/*---------------------------------- UTILS -----------------------------------*/
/*----------------------------------------------------------------------------*/
void		dup2_err(t_shell *shell);
char		*remove_quotes(char *str);
void		builtin_err(t_shell *shell, char *err, char *input);
int			error(int cond, char *str, int ecode, t_shell *shell);
void		error_exit(int cond, char *str, int ecode);
void		set_err(t_shell *shell, char *str);
void		set_error_stat(int stat, t_token **lst);
void		free_2d(char **str);
char		*join_with_symbol(char *s1, char *s2, char c);
int			set_status(t_shell *shell, int stat);
void		search_heredoc(t_shell *shell, t_token *lst);
void		fake_heredoc(t_shell *shell, t_token *lim);
void		here_wait(t_shell *shell, pid_t *pid);
void		set_attr(int mode);
void		hd_wait(t_shell *shell, int *pid, t_redir **node);
void		heredoc(t_shell *shell, t_input *word, t_redir *node);
int			is_env_name(char *s);
void		ft_append(char **str, char c);
char		*find_dollar(char *str, int flag);
char		*expand_vars(t_shell *shell, char *str, int flag);
void		expand(t_shell *shell, t_ast_node *node);
t_input		*ft_new_arg(char *name);
t_input		*ft_last_arg(t_input *args);
void		ft_args_add(t_input **args, t_input *new);
int			search_wildcard(char *str);
void		replace_wildcard(t_input **arg);
int			wd_match(char *pattern, char *text);
t_input		*read_directory(t_input *args);
void		clean(t_shell *shell);

#endif
