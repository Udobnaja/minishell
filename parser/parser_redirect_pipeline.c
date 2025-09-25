#include "parser_internal.h"

static t_redirect_type	prs_map_redirect_type(t_token_type type);
static t_io_stream		prs_map_io(t_redirect_type type);
static t_parser_status	prs_smpl_redirect_to_pipe(
							const t_token_type type,
							const t_word *word,
							t_shell *sh, t_cmd *cmd);
static t_parser_status	prs_heredoc_redirect_to_pipe(t_cmd *cmd, t_shell *sh, size_t *cur_heredoc);

t_parser_status prs_redirect_to_pipe(
					t_token_node *token_node,
					t_shell *sh, t_cmd *cmd,
					size_t *cur_heredoc)
{
	if (prs_is_simple_redirect(token_node->token->type))
		return (prs_smpl_redirect_to_pipe(
			token_node->token->type,
			&token_node->next->token->word,
			sh,
			cmd
		));
	return (
		prs_heredoc_redirect_to_pipe(cmd, sh, cur_heredoc)
	);
}

static t_parser_status	prs_smpl_redirect_to_pipe(
							const t_token_type type,
							const t_word *word,
							t_shell *sh, t_cmd *cmd
)
{
	t_parser_status status;
	char			*arg;
	t_redirect      *node;

	arg = NULL;
	status = prs_is_word_ambiguous(word, sh);
	if (status != PARSE_OK)
		return (status);
	status = prs_join_word(word, sh, &arg);
	if (status != PARSE_OK)
		return (status);
	node = ft_calloc(1, sizeof *node);
	if (!node)
	{
		free(arg);
		return (PARSE_ALLOC_ERROR);
	}
	node->type = prs_map_redirect_type(type);
	node->stream = prs_map_io(node->type);
	node->target.path = arg;
	pipeline_push_redirect(cmd, node);
	return (PARSE_OK);
}

static t_parser_status	prs_heredoc_redirect_to_pipe(
							t_cmd *cmd,
							t_shell *sh, size_t *cur_heredoc)
{
	t_redirect	*node;

	if (*cur_heredoc >= sh->heredoc_store->count)
		return (PARSE_HEREDOC_ERROR);
	node = ft_calloc(1, sizeof *node);
	if (!node)
		return (PARSE_ALLOC_ERROR);
	node->type = REDIR_HEREDOC;
	node->stream = IO_STDIN;
	node->target.fd = sh->heredoc_store->entries[*cur_heredoc].fd;
	pipeline_push_redirect(cmd, node);
	sh->heredoc_store->entries[*cur_heredoc].fd = -1;
	*cur_heredoc += 1;
	return (PARSE_OK);
}

static t_redirect_type prs_map_redirect_type(t_token_type type)
{
	if (type == T_REDIR_IN)
		return (REDIR_IN);
	if (type == T_REDIR_OUT)
		return (REDIR_OUT);
	if (type == T_REDIR_APP)
		return (REDIR_APPEND);
	return (REDIR_HEREDOC);
}

static t_io_stream prs_map_io(t_redirect_type type)
{
	if (type == REDIR_IN || type == REDIR_HEREDOC)
		return (IO_STDIN);
	return (IO_STDOUT);
}