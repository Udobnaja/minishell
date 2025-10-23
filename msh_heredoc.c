#include "minishell.h"

static int				msh_heredoc_has_expansion(t_token_node *node);
static char				*msh_heredoc_cpy_pieces(char *delimeter,
							t_token_node *node);
static char				*msh_heredoc_make_delimeter(t_token_node *node);
static t_heredoc_status	msh_process_heredoc(t_token_node *heredoc_node,
							t_shell *sh, t_heredoc_result *result);

t_heredoc_status	msh_prepare_heredocs(t_token_list *token_list, t_shell *sh)
{
	t_token_node		*cur;
	t_heredoc_status	status;
	t_heredoc_result	result;

	cur = token_list->head;
	while (cur)
	{
		if (cur->token->type == T_HEREDOC)
		{
			status = msh_process_heredoc(cur, sh, &result);
			if (status != HEREDOC_OK)
				return (status);
			if (heredoc_store_add(sh->heredoc_store, result.fd) != 0)
			{
				close(result.fd);
				err_print(ERR_HEREDOC, HEREDOC_ALLOC_ERROR, (t_err_payload){0});
				return (HEREDOC_ALLOC_ERROR);
			}
		}
		cur = cur->next;
	}
	return (HEREDOC_OK);
}

static t_heredoc_status	msh_process_heredoc(t_token_node *heredoc_node,
		t_shell *sh, t_heredoc_result *result)
{
	char			*delimeter;
	t_err_payload	payload;
	int				has_expansion;

	has_expansion = msh_heredoc_has_expansion(heredoc_node->next);
	delimeter = msh_heredoc_make_delimeter(heredoc_node->next);
	payload = (t_err_payload){0};
	if (!delimeter)
	{
		err_print(ERR_HEREDOC, HEREDOC_ALLOC_ERROR, payload);
		return (HEREDOC_ALLOC_ERROR);
	}
	*result = heredoc_write_to_tmpfile(sh, delimeter, has_expansion);
	free(delimeter);
	if (result->status != HEREDOC_OK)
	{
		if (result->status == HEREDOC_ABORTED)
			return (result->status);
		payload.errno_val = result->errno_val;
		err_print(ERR_HEREDOC, result->status, payload);
		return (result->status);
	}
	return (HEREDOC_OK);
}

static int	msh_heredoc_has_expansion(t_token_node *node)
{
	size_t	i;

	i = 0;
	while (i < node->token->word.count)
	{
		if (node->token->word.pieces[i].quote != NONE)
			return (0);
		i++;
	}
	return (1);
}

static char	*msh_heredoc_cpy_pieces(char *delimeter, t_token_node *node)
{
	size_t	len;
	size_t	counter;
	size_t	i;

	len = 0;
	counter = 0;
	i = 0;
	while (i < node->token->word.count)
	{
		len = ft_strlen(node->token->word.pieces[i].text);
		ft_memcpy(delimeter + counter, node->token->word.pieces[i].text, len);
		counter += len;
		i++;
	}
	delimeter[counter] = '\0';
	return (delimeter);
}

static char	*msh_heredoc_make_delimeter(t_token_node *node)
{
	size_t	i;
	size_t	len;
	char	*delimeter;

	i = 0;
	len = 0;
	while (i < node->token->word.count)
	{
		len += ft_strlen(node->token->word.pieces[i].text);
		i++;
	}
	delimeter = malloc(len + 1);
	if (!delimeter)
		return (NULL);
	return (msh_heredoc_cpy_pieces(delimeter, node));
}
