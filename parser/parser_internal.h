#ifndef PARSER_INTERNAL_H
# define PARSER_INTERNAL_H

# include "pipeline.h"

# include "parser.h"
# include "shell.h"
# include "expansion/expansion.h"

typedef struct s_ambiguous_state {
	size_t	in_word;
	size_t	leading_sp;
	size_t	trailing_sp;

}	t_ambiguous_state;

typedef enum e_trim_state {
	LEADING,
	MIDDLE,
	TRAILING
} t_trim_state;

int				prs_is_redirect(t_token_type type);
int				prs_is_simple_redirect(t_token_type type);
int				prs_count_words(char const *s, const char *set);
int				prs_has_only_spaces(char const *s, const char *set);
t_parser_status	prs_count_word_len(const t_word *word, t_shell *sh, size_t *total);
t_parser_status	prs_join_word(const t_word *word, t_shell *sh, char **new_word);
t_parser_status	prs_is_word_ambiguous(const t_word *word, t_shell *sh);
t_parser_status	prs_count_piece_len(const t_piece	*piece, t_shell *sh, size_t *total);
void			prs_append_sngl_quoted(const char *str, t_trim_state *st, char **new_word);
void			prs_trim_expansion(char *start, t_trim_state *st,  int quoted, char **new_word);

#endif