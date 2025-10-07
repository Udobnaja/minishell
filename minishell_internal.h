#ifndef MINISHELL_INTERNAL_H
# define MINISHELL_INTERNAL_H

# include "minishell.h"

int				msh_stage_init_tokens(t_token_list **token_list, t_msh_parse_result	*status);
int				msh_stage_tokenize(const char *str, t_token_list *token_list, t_msh_parse_result	*status);
int				msh_stage_preparse(t_token_list *token_list, t_msh_parse_result	*status);
int				msh_stage_heredoc(t_shell *shell, t_token_list *token_list, t_msh_parse_result	*status);
int				msh_stage_pipeline(t_shell *shell, t_pipeline *pipeline, t_token_list *token_list, t_msh_parse_result	*status);

#endif
