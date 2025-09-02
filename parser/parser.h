#ifndef PARSER_H
# define PARSER_H

# include "libft.h"

# include "pipeline.h"
# include "./lexer/lexer.h"

typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_REDIR_APP,
	T_HEREDOC,
	T_EOF
}	t_token_type;

typedef enum e_quote
{
	NONE,
	SGL,
	DBL
}	t_quote;

typedef struct s_piece
{
	char	*text;
	t_quote	quote;
}	t_piece;

typedef struct s_word
{
	t_piece	*pieces;
	int		count;
}	t_word;

typedef struct s_token
{
	t_token_type	type;
	t_word			word;
}	t_token;

typedef enum e_parser_status {
	PARSE_OK = 0,
	PARSE_UNEXPECTED_TOKEN,
	PARSE_ALLOC_ERROR,
	PARSE_LEX_ERROR
}	t_parser_status;

typedef struct s_token_node {
    t_token        		*token;
    struct s_token_nod	*next;
} t_token_node;

typedef struct s_token_list {
    t_token_node  *head;
    t_token_node  *tail;
    size_t      size;
} t_token_list;

#endif
