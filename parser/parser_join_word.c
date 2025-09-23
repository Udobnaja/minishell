#include "parser_internal.h"

static t_parser_status	prs_append_expandable(const char *str, t_shell *sh, char **new_word);
static t_parser_status	prs_append_expanded_key(const char *str, t_shell *sh, size_t *consumed, char **new_word);
static void				prs_append_until_expansion(const char *str, size_t *consumed, char **new_word);

t_parser_status	prs_join_word(const t_word *word, t_shell *sh, char **new_word)
{
	size_t			i;
	size_t			total;
	char			*nw_p;
	size_t			len;
	t_parser_status	status;
	
	if (prs_count_word_len(word, sh, &total) != PARSE_OK)
		return (PARSE_ALLOC_ERROR);
	*new_word = malloc(total + 1);
	if (!*new_word)
		return (PARSE_ALLOC_ERROR);
	nw_p = *new_word;	
	i = 0;	
	while (i < word->count)
	{
		if (word->pieces[i].quote == SGL)
		{
			len = ft_strlen(word->pieces[i].text);
			ft_memcpy(nw_p, word->pieces[i].text, len);
			nw_p += len;
		}
		else
		{
			status = prs_append_expandable(word->pieces[i].text, sh, &nw_p);
			if (status != PARSE_OK) {
				free(*new_word);
				*new_word = NULL;
				return (status);
			}
		}
		i++;
	}
	*nw_p = '\0';
	return (PARSE_OK);
}

static t_parser_status	prs_append_expandable(const char *str, t_shell *sh, char **new_word)
{
	size_t	j;
	t_parser_status status;

	j = 0;
	while (str[j])
	{
		prs_append_until_expansion(str + j, &j, new_word);
		if (str[j] == '$')
		{
			status = prs_append_expanded_key(str + j, sh, &j, new_word);
			if (status != PARSE_OK)
				return (status);
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
	ft_memcpy(*new_word, expanded, len);
	*new_word += len; 	
	*consumed += (ft_strlen(key));	
	free(key);
	free(expanded);
	return (PARSE_OK);
}

static void	prs_append_until_expansion(const char *str, size_t *consumed, char **new_word)
{
	size_t i;

	i = 0;
	while (str[i] && str[i] != '$')
		i++;
	ft_memcpy(*new_word, str, i);
	*new_word += i; 
	*consumed += i;
}