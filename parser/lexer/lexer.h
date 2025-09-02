#ifndef LEXER_H
# define LEXER_H

# include "libft.h"

typedef enum e_lex_status {
	LEX_OK = 0,
	LEX_UNMATCHED_QUOTES,
	LEX_ALLOC_ERROR,
	LEX_INVALID_SEQ
	// LEX_UNSUPPORTED_TOKEN
}	t_lex_status;

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

typedef struct s_token_node {
    t_token        		*token;
    struct s_token_node	*next;
} t_token_node;

typedef struct s_token_list {
    t_token_node  *head;
    t_token_node  *tail;
    size_t      size;
} t_token_list;

typedef struct s_lex_result
{
	t_lex_status	status;
	char			invalid_char;
}	t_lex_result;

t_lex_result lex_tokenize(const char *str, t_token_list	*token_list);

#endif