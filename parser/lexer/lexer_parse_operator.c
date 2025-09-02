#include "lexer_internal.h"

static t_token_type	lex_redir_type(int c, int len);

t_lex_parse_result	lex_parse_operator(const char *str, t_token *token)
{
	const char operator = str[0];
	if (str[0] == '|')
	{
		if (str[1] && str[1] == operator)
			return (lex_parse_err(LEX_INVALID_SEQ, operator));
		token->type = T_PIPE;
		return (lex_parse_ok(1));
	}

	if (str[1] && str[1] == operator)
	{
		if (str[2] && lex_is_operator(str[2]))
			return (lex_parse_err(LEX_INVALID_SEQ, str[2]));
		token->type = lex_redir_type(operator, 2);
		return (lex_parse_ok(2));   
	}
	
	if (str[1] && lex_is_operator(str[1]))
		return (lex_parse_err(LEX_INVALID_SEQ, str[1]));
	token->type = lex_redir_type(operator, 1);
	return (lex_parse_ok(1));    
}

static t_token_type	lex_redir_type(int c, int len)
{
	if (c == '>') {
		if (len == 1) 
			return (T_REDIR_OUT);
		return (T_REDIR_APP);
	}
	if (c == '<') {
		if (len == 1)
			return (T_REDIR_IN);
		return (T_HEREDOC);
	}
	return T_EOF;
}
