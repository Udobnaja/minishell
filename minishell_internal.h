#ifndef MINISHELL_INTERNAL_H
# define MINISHELL_INTERNAL_H

# include "minishell.h"

int				msh_stage_init_tokens(t_token_list **token_list,
					t_msh_parse_result *status);
int				msh_stage_tokenize(const char *str, t_token_list *token_list,
					t_msh_parse_result *status);
int				msh_stage_preparse(t_token_list *token_list,
					t_msh_parse_result *status);
int				msh_stage_heredoc(t_shell *shell, t_token_list *token_list,
					t_msh_parse_result *status);
int				msh_stage_pipeline(t_shell *shell, t_pipeline *pipeline,
					t_token_list *token_list, t_msh_parse_result *status);
t_parser_status	msh_pipeline_init(t_token_list *token_list,
					t_pipeline *pipeline);
const char		*msh_get_prompt(const char *name);
int				msh_has_only_spaces(char *str);
void			msh_clean_and_exit(t_shell *shell, int exit_status,
					int is_interactive);
void			msh_run_pipeline(char *line, t_shell *sh, int is_interactive);
int				msh_signal_check(char *line, t_shell *sh);

#endif
