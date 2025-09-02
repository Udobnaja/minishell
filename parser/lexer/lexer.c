#include "parser.h"

void	lex_push_node(t_token_list *list, t_token_node *node)
{
	if (!list->head)
	{
		list->head = node;
		list->tail = node;
	}
	else {
		list->tail->next = node;
		list->tail = node; 
	}
	list->size++;
}

t_token_node	*lex_create_node(t_token *token)
{
	t_token_node	*node;

	node = malloc(sizeof(t_token_node));
	if (!node)
		return (NULL);
	node->token = token;
	node->next = NULL;
	return (node);
}

static t_lex_parse_result lex_parse_ok(int consumed)
{
    t_lex_parse_result result;

	result.status = LEX_OK;
	result.payload.consumed = consumed;
	return (result);
}

static t_lex_parse_result lex_parse_err(t_lex_status e, char invalid_char)
{
    t_lex_parse_result result;

	result.status = e;
	result.payload.invalid_char = invalid_char;
	return (result);
}

static t_lex_count_result lex_count_error(t_lex_status e, char invalid_char) {
	t_lex_count_result result;
	
	result.status = e;
	result.payload.invalid_char = invalid_char;
	return (result);
}

static t_lex_count_result lex_count_ok(int count) {
	t_lex_count_result result;
	
	result.status = LEX_OK;
	result.payload.count = count;
	return (result);
}

static t_lex_token_result lex_token_ok(t_token *token, size_t consumed)
{
    t_lex_token_result result;

    result.status = LEX_OK;
    result.payload.success.token = token;
    result.payload.success.consumed = consumed;
    return result;
}

static t_lex_token_result lex_token_err(t_lex_status status, char invalid_char) {
    t_lex_token_result result;

    result.status = status;
    result.payload.error.invalid_char = invalid_char;
    return result;
}

static t_lex_result lex_ok()
{
    t_lex_result result;

	result.status = LEX_OK;
	result.invalid_char = 0;
	return (result);
}

static t_lex_result lex_err(t_lex_status e, char invalid_char)
{
    t_lex_result result;

	result.status = e;
	result.invalid_char = invalid_char;
	return (result);
}

t_lex_count_result lex_count_word_pieces(const char *str)
{
	int		count;
	size_t	i;
	char	q;

	count = 0;
	i = 0;
	while (str[i] && !ft_isspace(str[i]))
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			q = str[i];
			i++;
			while (str[i] && str[i] != q)
				i++;
			if (str[i] == '\0')
				return (lex_count_error(LEX_UNMATCHED_QUOTES, q));
			i++;
		} else
			while (str[i] && (!ft_isspace(str[i]) && str[i] != '\'' && str[i] != '"'))
				i++;
		count++;
	}		
	return (lex_count_ok(count));	
}

t_lex_parse_result lex_parse_piece(const char *str, t_piece *piece, t_quote quote, int (*stop)(int c))
{
	size_t i;

	i = 0;
	while (str[i] && (!stop(str[i])))
		i++;
	piece->text = malloc(i + 1);
	if (!piece->text)
		return (lex_parse_err(LEX_ALLOC_ERROR, 0));
	ft_memcpy(piece->text, str, i);
	piece->text[i] = '\0';
	piece->quote = quote;
	return (lex_parse_ok(i));
}

static int stop_space_or_quote(int c)
{
    if (c == '\'' || c == '"')
        return (1);
    return (ft_isspace(c));
}

static int stop_until_sgl(int c) 
{
	return (c == '\'');
}
static int stop_until_dbl(int c)
{
	return (c == '"');
}

t_quote lex_quote_type(int c)
{
	if (c == '"')
		return (DBL);
	if (c == '\'')
		return (SGL);
	return (NONE);	
}

static int (*lex_stop_resolver(t_quote quote))(int)
{
	if (quote == SGL)
		return stop_until_sgl;
	if (quote == DBL)
		return stop_until_dbl;
	return stop_space_or_quote;
}

t_lex_parse_result lex_parse_word(const char *str, t_token *token)
{
	size_t			i;
	size_t			piece_i;
	t_quote 		quote;
	t_lex_parse_result	result;
	int				(*stop)(int);
	
	i = 0;
	piece_i = 0;
	while (str[i] && !ft_isspace(str[i]))
	{
		quote = lex_quote_type(str[i]);
		stop = lex_stop_resolver(quote);
		if (quote != NONE)
			i++;
		result = lex_parse_piece(str + i, &token->word.pieces[piece_i], quote, stop);
		if (result.status != LEX_OK)
			return (result);
		i += result.payload.consumed;	
		if (quote != NONE)
			i++;
		piece_i++;
	}	
	return (lex_parse_ok(i));
}

t_lex_status init_word_token(t_token *token, int pieces_count)
{
	ft_bzero(token, sizeof(t_token));
	token->type = T_WORD;
	token->word.count = pieces_count;
	token->word.pieces = malloc((pieces_count) * sizeof(t_piece));
	if (!token->word.pieces)
		return (LEX_ALLOC_ERROR);
	return (LEX_OK);
}

t_lex_token_result create_word_token(const char *str)
{
	t_lex_parse_result			parse_res;
	t_lex_status				init_status;
	t_token						*token;
	const t_lex_count_result	count_res = lex_count_word_pieces(str);
	
	if (count_res.status != LEX_OK)
		return (lex_token_err(count_res.status, count_res.payload.invalid_char));
	token = calloc(1, sizeof(t_token));
	if (!token)
        return lex_token_err(LEX_ALLOC_ERROR, 0);
	init_status = init_word_token(token, count_res.payload.count);
	if (init_status != LEX_OK)
		//     free(token); 
		return lex_token_err(init_status, 0);
	parse_res = lex_parse_word(str, token);
	if (parse_res.status != LEX_OK)
		//     free(token); 
		return lex_token_err(parse_res.status, 0);
	return (lex_token_ok(token, parse_res.payload.consumed));
}

t_lex_result lex_tokenize(const char *str, t_token_list	*token_list)
{
	size_t			i;
	t_lex_token_result	token_result;;
	t_token_node	*token_node;
	t_lex_result	*result;

	i = 0;
	while (str[i])
	{
		while (str[i] && ft_isspace(str[i]))
			i++;
		if (str[i] == '>' || str[i] == '<' || str[i] == '|')
		{
			// TODO: create tokens with redir adn pipes + valid
			i++;
		} else {
			token_result = create_word_token(str + i);
			if (token_result.status != LEX_OK)
				 /* TODO: lex_list_destroy(token_list); */
				return (lex_err(token_result.status, token_result.payload.error.invalid_char));
			i += token_result.payload.success.consumed;			
		}
		token_node = lex_create_node(token_result.payload.success.token);
		if (!token_node)
			//     free(token_result.payload.success.token);
			//  /* TODO: lex_list_destroy(token_list); */
			return lex_err(LEX_ALLOC_ERROR, 0);
		lex_push_node(token_list, token_node);	
	}
	return (lex_ok());
}