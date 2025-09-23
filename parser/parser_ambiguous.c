#include "parser_internal.h"

static t_parser_status	prs_is_expanded_ambiguous(
	const char *expanded,
	const char	*set,
	t_ambiguous_state *amb_state
);

static t_parser_status	prs_is_expandble_ambiguous(
	const char *str, t_shell *sh,
	t_ambiguous_state *amb_state,
	size_t *consumed
);

static t_parser_status	prs_is_unquoted_ambiguous(
    const char *str,
    t_shell *sh,
    t_ambiguous_state *amb_state
);

t_parser_status prs_is_word_ambiguous(const t_word *word, t_shell *sh)
{
	size_t				i;
	t_ambiguous_state	amb_state;
	t_parser_status		status;

	i = 0;
	ft_bzero(&amb_state, sizeof amb_state);
	while (i < word->count)
	{
		if (word->pieces[i].quote != NONE)
		{
			amb_state.in_word = 1;
			if (amb_state.trailing_sp)
				return (PARSE_AMBIGUOUS_REDIRECT);
			i++;
			continue;
		}
		status = prs_is_unquoted_ambiguous(word->pieces[i].text, sh, &amb_state);
		if (status != PARSE_OK)
			return (status);
		i++;
	}
	if (!amb_state.in_word)
		return (PARSE_AMBIGUOUS_REDIRECT);

	return (PARSE_OK);
}

static t_parser_status	prs_is_expanded_ambiguous(
	const char *expanded,
	const char	*set,
	t_ambiguous_state *amb_state
)
{
	int		only_spaces;
	size_t	len;

	if (prs_count_words(expanded, set) > 1)
		return (PARSE_AMBIGUOUS_REDIRECT);
	only_spaces = prs_has_only_spaces(expanded, set);
	if (!only_spaces)
	{
		len = ft_strlen(expanded);
		amb_state->leading_sp = ft_strchr(set, (unsigned char)expanded[0]) != NULL;
		if (amb_state->in_word && (amb_state->leading_sp || amb_state->trailing_sp))
			return (PARSE_AMBIGUOUS_REDIRECT);
		if (ft_strchr(set, expanded[len - 1]))
			amb_state->trailing_sp = 1;
	}
	else if (ft_strlen(expanded) > 0 && amb_state->in_word)
		amb_state->trailing_sp = 1;
	if (!only_spaces)
		amb_state->in_word = 1;
	return (PARSE_OK);
}

static t_parser_status	prs_is_expandble_ambiguous(
	const char *str, t_shell *sh,
	t_ambiguous_state *amb_state,
	size_t *consumed
)
{
	char			*key;
	char			*expanded;
	t_parser_status	status;
	const char		*set = " \t\n";

	key = expn_dup_env_key(str);
	if (!key)
		return (PARSE_ALLOC_ERROR);	
	expanded = expn_expand(key, sh->env_store, sh->last_status);
	if (!expanded)
	{
		free(key);
		return (PARSE_ALLOC_ERROR);
	}
	status = prs_is_expanded_ambiguous(expanded, set, amb_state);
	if (status != PARSE_OK)
		return (free(key), free(expanded), status);
	*consumed += ft_strlen(key);
	return (free(key), free(expanded), PARSE_OK);
}

static t_parser_status	prs_is_unquoted_ambiguous(const char *str, t_shell *sh, t_ambiguous_state *amb_state)
{
	size_t			j;
	t_parser_status status;
	
	j = 0;
	while (str[j])
	{
		while (str[j] && str[j] != '$')
		{
			amb_state->in_word = 1;
			if (amb_state->trailing_sp)
				return (PARSE_AMBIGUOUS_REDIRECT);
			j++;
		}		
		if (str[j] == '$')
		{
			status = prs_is_expandble_ambiguous(str + j, sh, amb_state, &j);
			if (status != PARSE_OK)
				return (status);
		}
	}
	return (PARSE_OK);
}