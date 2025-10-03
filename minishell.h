#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>

# include "shell.h"
# include "pipeline.h"
# include "executor.h"
# include "heredoc.h"
# include "errors.h"
# include "parser.h"

# define SHELL_NAME "minishell"
# define HEREDOC_MAX 16

typedef enum e_msh_parse_domain {
	MPR_OK = 0,
	MPR_LEX,
	MPR_PARSE,
	MPR_HEREDOC
}   t_msh_parse_domain;

typedef union u_msh_parse_code
{
	t_lex_status		lex;
	t_parser_status		parse;
	t_heredoc_status	heredoc;
}	t_msh_parse_code;

typedef struct s_msh_parse_result {
	t_msh_parse_domain	domain;
	t_msh_parse_code	code; 
}	t_msh_parse_result;

int					msh_init(t_shell *shell, char **envp, const char *sh_name);
t_msh_parse_result	msh_parse(const char *str, t_shell *shell, t_pipeline *pipeline);
t_heredoc_status	msh_prepare_heredocs(t_token_list *token_list, t_shell *sh);
t_heredoc_status	msh_pre_heredocs(t_token_list *token_list);
t_parser_status		msh_pipeline(t_token_list *token_list, t_shell *shell, t_pipeline *pipeline);

#endif
