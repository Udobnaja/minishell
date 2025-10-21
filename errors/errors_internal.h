#ifndef ERRORS_INTERNAL_H
# define ERRORS_INTERNAL_H

# include <stdio.h>
# include <string.h>

# include "libft.h"
# include "env.h"
# include "parser.h"
# include "expansion/expansion.h"
# include "heredoc.h"
# include "executor.h"
# include "errors.h"

# define ERROR_MAX_BUFFER 1024

typedef struct s_error
{
	t_err_domain	domain;
	int				code;
	t_err_payload	payload;
	char			msg[256];
}	t_error;

const char  *err_env_msg(int code);
const char  *err_lexer_msg(int code);
const char  *err_parser_msg(int code);
const char  *err_heredoc_msg(int code);
t_error     err_create(t_err_domain domain, int code, t_err_payload payload);
const char 	*err_exec_msg(int code);
int			err_has_payload(const t_error *error);
void		err_format_msg(char *buf,  size_t *pos, const char *format, ...);

#endif