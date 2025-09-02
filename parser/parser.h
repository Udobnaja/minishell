#ifndef PARSER_H
# define PARSER_H

# include "pipeline.h"
# include "./lexer/lexer.h"

typedef enum e_parser_status {
	PARSE_OK = 0,
	PARSE_UNEXPECTED_TOKEN,
	PARSE_ALLOC_ERROR,
	PARSE_LEX_ERROR
}	t_parser_status;

#endif
