#include "minishell.h"

static int	msh_heredoc_has_expansion(t_token_node *node);
static char *msh_heredoc_cpy_pieces(char *delimetr, t_token_node *node);
static char *msh_heredoc_make_delimetr(t_token_node *node);

t_parser_status msh_prepare_heredocs(t_token_list *token_list, t_shell *sh)
{
	t_token_node		*cur;
	t_heredoc_result	result;
	int					has_expansion;
	char				*delimetr;
	t_err_payload	payload;

	payload = (t_err_payload){0};
	cur = token_list->head;
	while (cur)
	{
		if (cur->token->type == T_HEREDOC)
		{
			has_expansion = msh_heredoc_has_expansion(cur->next);
			delimetr = msh_heredoc_make_delimetr(cur->next);
			if (!delimetr)
			{
				err_print(ERR_HEREDOC, HEREDOC_ALLOC_ERROR, payload);
				return (PARSE_ALLOC_ERROR);
			}
				
			result = heredoc_write_to_tmpfile(sh, delimetr, has_expansion);
			if (result.status != HEREDOC_OK)
			{
				free(delimetr);
				payload.errno_val = result.errno_val;
				err_print(ERR_HEREDOC, result.status, payload);
				return (PARSE_HEREDOC_ERROR);
			}
			free(delimetr);
			// TODO: write fd
		}
		cur = cur->next;
	}    
	return (PARSE_OK);
}

static int msh_heredoc_has_expansion(t_token_node *node)
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

static char *msh_heredoc_cpy_pieces(char *delimetr, t_token_node *node)
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
		ft_memcpy(delimetr + counter, node->token->word.pieces[i].text, len);
		counter += len;
		i++;
	}
	delimetr[counter] = '\0';
	return (delimetr);
}

static char *msh_heredoc_make_delimetr(t_token_node *node)
{
	size_t	i;
	size_t	len;
	char	*delimetr;

	i = 0;
	len = 0;
	while (i < node->token->word.count)
	{
		len += ft_strlen(node->token->word.pieces[i].text);
		i++;
	}
	delimetr = malloc(len + 1);
	if (!delimetr)
		return (NULL);
	return (msh_heredoc_cpy_pieces(delimetr, node));
}
