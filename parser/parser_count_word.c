#include "parser_internal.h"

static t_parser_status	prs_count_expanded_key_len(const char *str, t_shell *sh,
							size_t *consumed, size_t *total);
static t_parser_status	prs_count_expandable_len(const char *str, t_shell *sh,
							size_t *total);

t_parser_status	prs_count_piece_len(const t_piece *piece, t_shell *sh,
		size_t *total)
{
	t_parser_status	status;

	if (piece->quote == SGL)
		*total += ft_strlen(piece->text);
	else
	{
		status = prs_count_expandable_len(piece->text, sh, total);
		if (status != PARSE_OK)
			return (status);
	}
	return (PARSE_OK);
}

t_parser_status	prs_count_word_len(const t_word *word, t_shell *sh,
		size_t *total)
{
	size_t			i;
	t_parser_status	status;

	*total = 0;
	i = 0;
	while (i < word->count)
	{
		status = prs_count_piece_len(&word->pieces[i], sh, total);
		if (status != PARSE_OK)
			return (status);
		i++;
	}
	return (PARSE_OK);
}

static t_parser_status	prs_count_expanded_key_len(const char *str, t_shell *sh,
		size_t *consumed, size_t *total)
{
	char	*key;
	char	*expanded;

	key = expn_dup_env_key(str);
	if (!key)
		return (PARSE_ALLOC_ERROR);
	expanded = expn_expand(key, sh->env_store, sh->last_status);
	if (!expanded)
	{
		free(key);
		return (PARSE_ALLOC_ERROR);
	}
	*total += ft_strlen(expanded);
	*consumed += ft_strlen(key);
	free(key);
	free(expanded);
	return (PARSE_OK);
}

static t_parser_status	prs_count_expandable_len(const char *str, t_shell *sh,
		size_t *total)
{
	size_t			j;
	t_parser_status	status;

	j = 0;
	while (str[j])
	{
		if (str[j] == '$')
		{
			status = prs_count_expanded_key_len(str + j, sh, &j, total);
			if (status != PARSE_OK)
				return (status);
		}
		else
		{
			j++;
			*total += 1;
		}
	}
	return (PARSE_OK);
}
