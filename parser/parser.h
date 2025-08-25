#ifndef PARSER_H
# define PARSER_H

# include "../pipeline/pipeline.h"

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

#endif
