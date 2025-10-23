#include "minishell_internal.h"

static int				msh_process_word_token(const t_word *word, t_shell *sh,
							t_cmd *cmd, t_parser_status *status);
static int				msh_process_pipe_token(t_cmd *cmd,
							t_parser_status *status);
static t_parser_status	msh_process_token(t_token_node *token_node,
							t_shell *shell, t_cmd *cmd, size_t *cur_heredoc);
static t_parser_status	msh_tokens_to_pipeline(t_token_list *token_list,
							t_shell *shell, t_pipeline *pipeline);

t_parser_status	msh_pipeline(t_token_list *token_list, t_shell *shell,
		t_pipeline *pipeline)
{
	t_parser_status	status;
	size_t			cmds_count;

	status = msh_pipeline_init(token_list, pipeline);
	if (status != PARSE_OK)
		return (status);
	cmds_count = pipeline->count;
	status = msh_tokens_to_pipeline(token_list, shell, pipeline);
	if (status != PARSE_OK)
		return (status);
	status = prs_finish_cmd(pipeline->cmds[cmds_count - 1]);
	if (status != PARSE_OK)
		err_print(ERR_PARSER, status, (t_err_payload){0});
	return (status);
}

static t_parser_status	msh_tokens_to_pipeline(t_token_list *token_list,
		t_shell *shell, t_pipeline *pipeline)
{
	t_parser_status	status;
	size_t			i;
	t_token_node	*cur;
	size_t			cur_heredoc;

	i = 0;
	cur_heredoc = 0;
	cur = token_list->head;
	while (cur)
	{
		status = msh_process_token(cur, shell, pipeline->cmds[i], &cur_heredoc);
		if (status != PARSE_OK)
			return (status);
		if (cur->token->type == T_PIPE)
			i++;
		if (cur->token->type != T_WORD && cur->token->type != T_PIPE)
			cur = cur->next->next;
		else
			cur = cur->next;
	}
	return (PARSE_OK);
}

static int	msh_process_word_token(const t_word *word, t_shell *sh, t_cmd *cmd,
		t_parser_status *status)
{
	*status = prs_word_to_argv(word, sh, cmd);
	if (*status != PARSE_OK)
	{
		err_print(ERR_PARSER, *status, (t_err_payload){0});
		return (0);
	}
	return (1);
}

static int	msh_process_pipe_token(t_cmd *cmd, t_parser_status *status)
{
	*status = prs_finish_cmd(cmd);
	if (*status != PARSE_OK)
	{
		err_print(ERR_PARSER, *status, (t_err_payload){0});
		return (0);
	}
	return (1);
}

static t_parser_status	msh_process_token(t_token_node *token_node,
		t_shell *shell, t_cmd *cmd, size_t *cur_heredoc)
{
	t_parser_status	status;

	if (token_node->token->type == T_WORD)
	{
		if (!msh_process_word_token(&token_node->token->word, shell, cmd,
				&status))
			return (status);
	}
	else if (token_node->token->type == T_PIPE)
	{
		if (!msh_process_pipe_token(cmd, &status))
			return (status);
	}
	else
	{
		status = prs_redirect_to_pipe(token_node, shell, cmd, cur_heredoc);
		if (status != PARSE_OK)
		{
			err_print(ERR_PARSER, status, (t_err_payload){0});
			return (status);
		}
	}
	return (PARSE_OK);
}
