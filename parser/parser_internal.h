#ifndef PARSER_INTERNAL_H
# define PARSER_INTERNAL_H

# include "pipeline.h"

# include "parser.h"
# include "shell.h"
# include "expansion.h"

int prs_is_redirect(t_token_type type);

#endif