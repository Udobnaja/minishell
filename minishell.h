#ifndef MINISHELL_H
# define MINISHELL_H

# include "errors.h"
# include "executor.h"
# include "heredoc.h"
# include "parser.h"
# include "pipeline.h"
# include "shell.h"

typedef enum e_msh_parse_domain
{
	MPR_OK = 0,
	MPR_LEX,
	MPR_PARSE,
	MPR_HEREDOC
}						t_msh_parse_domain;

typedef union u_msh_parse_code
{
	t_lex_status		lex;
	t_parser_status		parse;
	t_heredoc_status	heredoc;
}						t_msh_parse_code;

typedef struct s_msh_parse_result
{
	t_msh_parse_domain	domain;
	t_msh_parse_code	code;
}						t_msh_parse_result;

int						msh_init(t_shell *shell, char **envp,
							const char *sh_name);
t_msh_parse_result		msh_parse(const char *str, t_shell *shell,
							t_pipeline *pipeline);
t_heredoc_status		msh_prepare_heredocs(t_token_list *token_list,
							t_shell *sh);
t_heredoc_status		msh_pre_heredocs(t_token_list *token_list);
t_parser_status			msh_pipeline(t_token_list *token_list, t_shell *shell,
							t_pipeline *pipeline);
int						msh_parse_result_to_exit_status(
							t_msh_parse_result result);
void					msh_cleanup(t_shell *shell);

#endif
