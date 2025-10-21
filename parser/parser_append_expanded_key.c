#include "parser_internal.h"

static void prs_cpy_as_is(const char *expanded, char **new_word)
{
    size_t	len;
    len = ft_strlen(expanded);
    if (len > 0)
    {
        ft_memcpy(*new_word, expanded, len);
        *new_word += len; 
    }
}

static void prs_cpy_with_ifs_sep(const char *expanded, char **new_word)
{
    size_t	i;

    i = 0;
    while (expanded[i])
    {
        if (ft_strchr(IFS, (unsigned char)expanded[i]))
        {
            **new_word = FIELD_SEP;
            *new_word += 1;
        }
        else
        {
            **new_word = expanded[i];
            *new_word += 1;
        }
        i++;
    }
}

t_parser_status	prs_append_expanded_key(const t_piece *piece, t_shell *sh, size_t *consumed, char **new_word)
{
	char	*key;
	char	*expanded;
	const char *str = piece->text + *consumed;

	key = expn_dup_env_key(str);
	if (!key)
		return (PARSE_ALLOC_ERROR);	
	expanded = expn_expand(key, sh->env_store, sh->last_status);
	if (!expanded)
	    return (free(key), PARSE_ALLOC_ERROR);
	if (piece->quote != NONE)
        prs_cpy_as_is(expanded, new_word);
	else
        prs_cpy_with_ifs_sep(expanded, new_word);
	*consumed += (ft_strlen(key));	
	free(key);
	free(expanded);
	return (PARSE_OK);
}