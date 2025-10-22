#include "errors_internal.h"

static void	err_format_payload(const t_error *error, char *buf, size_t *pos);

void	err_print(t_err_domain domain, int code, t_err_payload payload)
{
	const t_error	error = err_create(domain, code, payload);
	char			buf[ERROR_MAX_BUFFER];
	size_t			pos;

	pos = 0;
	err_format_msg(buf, &pos, "%s: ", SHELL_NAME);
	if (err_has_payload(&error))
		err_format_payload(&error, buf, &pos);
	else
		err_format_msg(buf, &pos, error.msg);
	err_format_msg(buf, &pos, "\n");
	write(STDERR_FILENO, buf, pos);
}

static void	err_format_payload(const t_error *error, char *buf, size_t *pos)
{
	if ((error->domain == ERR_LEXER || error->domain == ERR_PARSER)
		&& error->payload.token)
		err_format_msg(buf, pos, error->msg, error->payload.token);
	else if (error->domain == ERR_HEREDOC && error->payload.errno_val)
		err_format_msg(buf, pos, error->msg,
			strerror(error->payload.errno_val));
	else if (error->domain == ERR_EXEC && error->payload.command
		&& error->payload.identifier)
		err_format_msg(buf, pos, error->msg, error->payload.command,
			error->payload.identifier);
	else if (error->domain == ERR_EXEC && error->payload.command
		&& error->payload.errno_val)
		err_format_msg(buf, pos, error->msg, error->payload.command,
			strerror(error->payload.errno_val));
	else if (error->domain == ERR_EXEC && error->payload.command)
		err_format_msg(buf, pos, error->msg, error->payload.command);
	else
		err_format_msg(buf, pos, error->msg);
}
