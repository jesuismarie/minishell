/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnazarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 15:59:04 by mnazarya          #+#    #+#             */
/*   Updated: 2024/02/18 20:12:01 by mnazarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINE_H
# define DEFINE_H

# define ERR_CD		"minishell: cd: HOME not set 🏠\n"
# define CD_2MUCH	"minishell: cd: too many arguments\n"
# define ERR_EXIT	"minishell: exit: too many arguments\n"
# define ERR_HIST	"minishell: history: too many arguments\n"
# define ERR_EXPORT	"minishell: export:"
# define ERR_UNSET	"minishell: unset:"
# define ERR_MSG	"minishell: syntax error near unexpected token `"
# define ERR_OP_B	"minishell: syntax error near unexpected token `('\n"
# define ERR_CL_B	"minishell: syntax error near unexpected token `)'\n"
# define ERR_AND	"minishell: syntax error near unexpected token `&'\n"
# define ERR_NL		"minishell: syntax error near unexpected token `newline'\n"
# define ERR_LIM	"here-document delimited by end-of-file (wanted `"
# define ERR_EOF	"minishell: unexpected EOF while looking for matching `"
# define ERR_SYN	"minishell: syntax error: unexpected end of file\n"
# define PERROR_MSG	"minishell"
# define HERE_MAX	"minishell: maximum here-document count exceeded\n"
# define SQ_ERR		"minishell: unclosed \' quote 🙃\n"
# define DQ_ERR		"minishell: unclosed \" quote 🙃\n"

# define EQUAL		'='
# define OR_IF		"||"
# define AND_IF		"&&"
# define LESS		'<'
# define GREAT		'>'
# define DLESS		"<<"
# define DGREAT		">>"
# define DOLLAR		'$'
# define PIPE		'|'
# define LBRACE		'('
# define RBRACE		')'
# define SQUOTES	'\''
# define DQUOTES	'\"'
# define WILDCARD	'*'

# define PS			"minishell 🥑$ "

# define OPERATORS	"()<>|&"

typedef enum e_token_type
{
	HEREDOC,
	APPEND,
	FILE_IN,
	FILE_OUT,
	ENV_PARAM,
	PIPE_OP,
	AND_OP,
	OR_OP,
	BRACE_OPEN,
	BRACE_CLOSE,
	WORD,
	ERROR
}	t_token_type;

typedef enum e_flags
{
	F_DOLLAR			= 1 << 0,
	F_SQUOTES			= 1 << 1,
	F_DQUOTES			= 1 << 2,
	F_ASSIGNMENT		= 1 << 3,
	F_MUL_DOLLAR		= 1 << 4,
	F_EXPANDED			= 1 << 5,
	F_DEL_QUOTES		= 1 << 6
}	t_flags;

typedef enum e_ast_node_type
{
	AST_LOGICAL_OP,
	AST_SUBSHELL,
	AST_PIPE,
	AST_COMMAND
}	t_ast_node_type;

typedef struct s_stack
{
	char			c;
	struct s_stack	*next;
	struct s_stack	*prev;
}	t_stack;

typedef struct s_input
{
	int				flag;
	char			*input;
	struct s_input	*next;
	struct s_input	*prev;
}	t_input;

typedef struct s_token
{
	t_token_type	type;
	t_input			*cmd;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;

typedef struct s_redir
{
	int				in_fd;
	int				out_fd;
	t_token_type	type;
	struct s_redir	*next;
	t_input			*filename;
}	t_redir;

typedef struct s_ast_node
{
	t_ast_node_type		type;
	int					in_fd;
	int					out_fd;
	t_redir				*red_lst;
	void				*node;
}	t_ast_node;

typedef struct s_pipe
{
	int			in_fd;
	int			out_fd;
	t_ast_node	*left;
	t_ast_node	*right;
}	t_pipe;

typedef struct s_operator
{
	int				in_fd;
	int				out_fd;
	t_token_type	type;
	t_ast_node		*left;
	t_ast_node		*right;
}	t_operator;

typedef struct s_cmd
{
	int		n;
	int		in_fd;
	int		out_fd;
	t_input	*name;
	t_input	*args;
	t_redir	*red_lst;
}	t_cmd;

typedef struct s_env
{
	char			*var_name;
	char			*var_value;
	int				hidden;
	struct s_env	*next;
	struct s_env	*prev;
}	t_env;

typedef struct s_shell
{
	int			err;
	char		**env;
	char		*prev;
	char		*line;
	t_ast_node	*tree;
	char		*hist;
	int			ex_code;
	t_env		*env_lst;
	char		*err_msg;
	int			history_fd;
	t_token		*token_head;
	int			all_fds[FOPEN_MAX];
	int			index;
}	t_shell;

#endif
