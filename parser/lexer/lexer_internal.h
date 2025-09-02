#ifndef LEXER_INTERNAL_H
# define LEXER_INTERNAL_H

# include "lexer.h"

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

void				lex_push_node(t_token_list *list, t_token_node *node);
t_token_node		*lex_create_node(t_token *token);
void				lex_free_node(t_token_node	*node);
void				lex_free_token(t_token	*token);
t_lex_token_result	lex_create_token(const char *str);
t_lex_parse_result	lex_parse_word(const char *str, t_token *token);
t_lex_parse_result	lex_parse_operator(const char *str, t_token *token);
int					(*lex_stop_resolver(t_quote quote))(int);
t_lex_count_result	lex_count_word_pieces(const char *str);
int					lex_is_operator(unsigned char c);
t_lex_parse_result	lex_parse_ok(int consumed);
t_lex_parse_result	lex_parse_err(t_lex_status e, char invalid_char);
t_lex_token_result	lex_token_ok(t_token *token, size_t consumed);
t_lex_token_result	lex_token_err(t_lex_status status, char invalid_char);

#endif