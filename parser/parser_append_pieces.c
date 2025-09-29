#include "parser_internal.h"

static size_t	prs_count_leading_spaces(char *start, const char *set, const size_t	len);
static void prs_define_trailing_state(const char *start, const char *end, const char *set, t_trim_state *st);

void	prs_append_sngl_quoted(const char *str, t_trim_state *st, char **new_word)
{
	const size_t len = ft_strlen(str);

	if (len == 0)
		return ;  
	*st = MIDDLE;
	ft_memcpy(*new_word, str, len);
	*new_word += len;
}

void	prs_trim_expansion(char *start, t_trim_state *st,  int quoted, char **new_word)
{
	const size_t	len = (size_t)(*new_word - start);
	const char		*set = " \t\n";
	size_t			leading_skip;

	if (len == 0)
		return;
	if (quoted)
	{
		*st = MIDDLE;
		return;
	}
	if (*st == LEADING)
	{
		leading_skip = prs_count_leading_spaces(start, set, len);
		if (leading_skip == len)
		{
			*new_word = start;
			return;
		}
		if (leading_skip > 0)
			ft_memmove(start, start + leading_skip, len - leading_skip);
		*new_word = start + (len - leading_skip);
	}
	prs_define_trailing_state(start, *new_word, set, st);
}

static size_t	prs_count_leading_spaces(char *start, const char *set, const size_t	len)
{
	size_t	i;
	i = 0;
	while (start[i] && i < len && ft_strchr(set, (unsigned char)start[i]))
		i++;
	return (i);
}

static void prs_define_trailing_state(const char *start, const char *end, const char *set, t_trim_state *st)
{
	const size_t len = (size_t)(end - start);

	if (len == 0)
        return ;
	if (ft_strchr(set, (unsigned char)start[len - 1]))
		*st = TRAILING;
	else
		*st = MIDDLE;
}
