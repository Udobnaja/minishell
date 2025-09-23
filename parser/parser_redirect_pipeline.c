#include "parser_internal.h"

static t_redirect_type prs_map_redirect_type(t_token_type type);
static t_io_stream prs_map_io(t_redirect_type type);

t_parser_status prs_redirect_to_pipe(
	t_token_node *token_node,
	t_shell *sh, t_cmd *cmd
)
{
    t_parser_status status;
    char			*arg;
    t_redirect      *node;

    arg = NULL;
    if (prs_is_simple_redirect(token_node->token->type))
    {
        status = prs_is_word_ambiguous(&token_node->next->token->word, sh);
        if (status != PARSE_OK)
            return (status);
        status = prs_join_word(&token_node->next->token->word, sh, &arg);
	    if (status != PARSE_OK)
		    return (status);
        node = ft_calloc(1, sizeof *node);
        if (!node)
        {
            free(arg);
            return (PARSE_ALLOC_ERROR);
        }
        node->type = prs_map_redirect_type(token_node->token->type);
        node->stream = prs_map_io(node->type);
        node->target.path = arg;
        pipeline_push_redirect(cmd, node);
    }
    else
    {
        // TODO: heredoc
    }

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