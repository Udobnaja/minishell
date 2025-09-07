#include "minishell.h"

static t_env_status	sh_env_init(t_shell *shell, char **envp, const char *sh_name);
static t_parser_status	sh_parse(const char *str);
static t_lex_status		sh_lex(const char *str, t_token_list *token_list);

const char *get_prompt(char *default_name)
{
	(void) (default_name);
	return ("minishell $ ");
}

int main(int argc, char **argv, char **envp)
{
	t_shell shell;
	const char *sh_name;

	if (argc > 0 && argv && argv[0] && argv[0][0] != '\0')
		sh_name = argv[0];
	else
		sh_name = SHELL_NAME;
	ft_bzero(&shell, sizeof(t_shell));
	if (sh_env_init(&shell, envp, sh_name) != ENV_OK)
		return (1);
	
	char *line;

	while(1)
	{
		line = readline(get_prompt(argv[0]));
		if (!line)
			break;
		if (*line)
		{
			add_history(line);
			sh_parse(line);
		}		
		free(line);
		line = NULL;
	}
	env_destroy(&shell.env_store);
	return (0);
}

static t_env_status	sh_env_init(t_shell *shell, char **envp, const char *sh_name)
{
	t_env_status	status;

	shell->env_store = env_store_create();
	if (!shell->env_store)
	{
		status = ENV_ALLOC_ERROR;
		err_print(ERR_ENV, status, (t_err_payload){0});
    	return (status);
	}
	status = env_init(shell->env_store, envp, sh_name);
	if (status != ENV_OK)
	{
		env_destroy(&shell->env_store);
		err_print(ERR_ENV, status, (t_err_payload){0});
		return (status);
	}
	return (status);
}

static const char *sh_token_to_char(t_token_type type)
{
	if (type == T_PIPE)
		return ("|");
	if (type == T_REDIR_IN)
		return ("<");
	if (type == T_REDIR_OUT)
		return (">");
	if (type == T_REDIR_APP)
		return (">>");
	if (type == T_HEREDOC)
		return ("<<");
	return ("?");
}

static t_parser_status	sh_pre_parse(t_token_list *token_list)
{
	t_err_payload		payload;
	t_pre_parse_result	result;
	char				token[3];

	payload = (t_err_payload){0};
	result = prs_pre_parse(token_list);
	if (result.status != PARSE_OK)
	{
		if (!result.token->next)
		{
			payload.token = "newline";
		} else {
			ft_strlcpy(token, sh_token_to_char(result.token->token->type), sizeof token);
			payload.token = token;
		}
		err_print(ERR_PARSER, result.status, payload);
		return (result.status);
	}
	return (PARSE_OK);
}

static t_parser_status	sh_parse(const char *str)
{
	t_token_list	*token_list;
	t_parser_status	status;
	t_lex_status	tokenize_status;

	token_list = ft_calloc(1, sizeof *token_list);
	if (!token_list)
	{
		status = PARSE_ALLOC_ERROR;
		err_print(ERR_LEXER, status, (t_err_payload){0});
		return (status);
	}
	tokenize_status = sh_lex(str, token_list);
	if (tokenize_status != LEX_OK)
	{
		lex_destroy_token_list(&token_list);
		return (PARSE_LEX_ERROR);
	}
	if (sh_pre_parse(token_list) != PARSE_OK)
	{
		lex_destroy_token_list(&token_list);
		return (PARSE_UNEXPECTED_TOKEN);
	}

	lex_destroy_token_list(&token_list);
	return (PARSE_OK);
}

static t_lex_status	sh_lex(const char *str, t_token_list *token_list)
{
	t_lex_result	result;
	t_err_payload	payload;
	char			token[2];

	payload = (t_err_payload){0};
	result = lex_tokenize(str, token_list);
	if (result.status != LEX_OK)
	{
		if (result.invalid_char)
		{
			token[0] = result.invalid_char;
			token[1] = '\0';
			payload.token = token;	
		}	
		err_print(ERR_LEXER, result.status, payload);
		return (result.status);
	}
	return (result.status);
}

