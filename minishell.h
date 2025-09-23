#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>

# include "shell.h"
# include "pipeline.h"
# include "executor.h"
# include "heredoc.h"
# include "errors.h"

# define SHELL_NAME "minishell"
# define HEREDOC_MAX 16

int				msh_init(t_shell *shell, char **envp, const char *sh_name);
t_parser_status	msh_parse(const char *str, t_shell *shell, t_pipeline *pipeline);
t_parser_status	msh_prepare_heredocs(t_token_list *token_list, t_shell *sh);
t_parser_status	msh_pre_heredocs(t_token_list *token_list);
t_parser_status	msh_pipline(t_token_list *token_list, t_shell *shell, t_pipeline *pipeline);

#endif
