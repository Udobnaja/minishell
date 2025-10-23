#include "errors_internal.h"

static const char	*(*err_resolver(t_err_domain domain))(int);

t_error	err_create(t_err_domain domain, int code, t_err_payload payload)
{
	t_error		error;
	const char	*message;
	const char	*(*resolver)(int);

	message = NULL;
	error.domain = domain;
	error.code = code;
	error.msg[0] = '\0';
	error.payload = payload;
	resolver = err_resolver(domain);
	if (resolver)
		message = resolver(code);
	if (message)
		ft_strlcpy(error.msg, message, sizeof error.msg);
	else
		ft_strlcpy(error.msg, "unknown error", sizeof error.msg);
	return (error);
}

static const char	*(*err_resolver(t_err_domain domain))(int c)
{
	static const char	*(*table[])(int) = {
	[ERR_NONE] = NULL,
	[ERR_ENV] = err_env_msg, [ERR_LEXER] = err_lexer_msg,
	[ERR_PARSER] = err_parser_msg, [ERR_HEREDOC] = err_heredoc_msg,
	[ERR_EXEC] = err_exec_msg};
	const size_t		size = sizeof table / sizeof * table;

	if (domain < 0 || domain >= size)
		return (NULL);
	return (table[domain]);
}
