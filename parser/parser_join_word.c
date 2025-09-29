#include "parser_internal.h"

static t_parser_status	prs_append_expandable(const t_piece *piece, t_shell *sh, t_trim_state *st, char **new_word);
static t_parser_status	prs_append_expanded_key(const char *str, t_shell *sh, size_t *consumed, char **new_word);
static void				prs_append_until_expansion(const char *str, size_t *consumed, t_trim_state *st, char **new_word);
static t_parser_status	prs_append_pieces(const t_word *word, t_shell *sh, char **new_word);

t_parser_status	prs_join_word(const t_word *word, t_shell *sh, char **new_word)
{
	size_t			total;
	char			*nw_p;
	t_parser_status	status;
	
	status = prs_count_word_len(word, sh, &total);
	if (status != PARSE_OK)
		return (status);
	*new_word = malloc(total + 1);
	if (!*new_word)
		return (PARSE_ALLOC_ERROR);
	nw_p = *new_word;
	status = prs_append_pieces(word, sh, &nw_p);
	if (status != PARSE_OK)
	{
		free(*new_word);
		*new_word = NULL;
		return (status);
	}
	return (PARSE_OK);
}

static t_parser_status	prs_append_pieces(const t_word *word, t_shell *sh, char **new_word)
{
	size_t			i;
	t_parser_status	status;
	t_trim_state	trim_state;
	char           *start; 

	i = 0;
	trim_state = LEADING;
	start = *new_word;  
	while (i < word->count)
	{
		if (word->pieces[i].quote == SGL)
			prs_append_sngl_quoted(word->pieces[i].text, &trim_state, new_word);
		else
		{
			status = prs_append_expandable(&word->pieces[i], sh, &trim_state, new_word);
			if (status != PARSE_OK)
				return (status);
		}
		i++;
	}
	if (trim_state == TRAILING)
		prs_trim_ending(start, new_word, " \t\n");
	**new_word = '\0';
	return (PARSE_OK);
}

static t_parser_status	prs_append_expandable(const t_piece *piece, t_shell *sh, t_trim_state *st, char **new_word)
{
	size_t			j;
	t_parser_status status;
	char            *start;

	j = 0;
	while (piece->text[j])
	{
		prs_append_until_expansion(piece->text + j, &j, st, new_word);
		if (piece->text[j] == '$')
		{
			start = *new_word;
			status = prs_append_expanded_key(piece->text + j, sh, &j, new_word);
			if (status != PARSE_OK)
				return (status);
			prs_trim_expansion(start, st, piece->quote != NONE, new_word);
		}
	}
	return (PARSE_OK);
}

static t_parser_status	prs_append_expanded_key(const char *str, t_shell *sh, size_t *consumed, char **new_word)
{
	char	*key;
	char	*expanded;
	size_t	len;

	key = expn_dup_env_key(str);
	if (!key)
		return (PARSE_ALLOC_ERROR);	
	expanded = expn_expand(key, sh->env_store, sh->last_status);
	if (!expanded)
	{
		free(key);
		return (PARSE_ALLOC_ERROR);
	}
	len = ft_strlen(expanded);
	if (len > 0)
	{
		ft_memcpy(*new_word, expanded, len);
		*new_word += len; 
	}	
	*consumed += (ft_strlen(key));	
	free(key);
	free(expanded);
	return (PARSE_OK);
}

static void	prs_append_until_expansion(const char *str, size_t *consumed,  t_trim_state *st, char **new_word)
{
	size_t i;

	i = 0;
	while (str[i] && str[i] != '$')
		i++;
	if (i == 0)
        return ;  
	ft_memcpy(*new_word, str, i);
	*new_word += i; 
	*consumed += i;
	if (*st != MIDDLE)
        *st = MIDDLE;
}