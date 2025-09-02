#ifndef LEXER_H
# define LEXER_H

typedef enum e_lex_status {
	LEX_OK = 0,
	LEX_UNMATCHED_QUOTES,
	LEX_ALLOC_ERROR,
	LEX_INVALID_SEQ
	// LEX_UNSUPPORTED_TOKEN
}	t_lex_status;

typedef union u_lex_count_payload
{
	int		count;
	char	invalid_char;
}	t_lex_count_payload;

typedef struct s_lex_count_result
{
	t_lex_status		status;
	t_lex_count_payload	payload;
}	t_lex_count_result;

typedef struct s_lex_token_success
{
	t_token *token;
	size_t  consumed;
}	t_lex_token_success;

typedef struct s_lex_token_error
{
	char	invalid_char;
}	t_lex_token_error;

typedef union u_lex_token_payload
{
	t_lex_token_success	success;
	t_lex_token_error	error;
}	t_lex_token_payload;

typedef struct s_lex_token_result
{
    t_lex_status		status;
    t_lex_token_payload	payload;
}	t_lex_token_result;

typedef union u_lex_parse_payload
{
	int		consumed;
	char	invalid_char;
}	t_lex_parse_payload;

typedef struct s_lex_parse_result
{
	t_lex_status	status;
	t_lex_parse_payload	payload;
}	t_lex_parse_result;

typedef struct s_lex_result
{
	t_lex_status	status;
	char			invalid_char;
}	t_lex_result;

t_lex_result lex_tokenize(const char *str, t_token_list	*token_list);

#endif