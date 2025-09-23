#ifndef PARSER_INTERNAL_H
# define PARSER_INTERNAL_H

# include "pipeline.h"

# include "parser.h"
# include "shell.h"
# include "expansion/expansion.h"

int					prs_is_redirect(t_token_type type);
t_parser_status		prs_count_word_len(const t_word *word, t_shell *sh, size_t *total);

#endif