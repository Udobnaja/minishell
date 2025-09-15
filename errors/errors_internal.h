#ifndef ERRORS_INTERNAL_H
# define ERRORS_INTERNAL_H

# include <stdio.h>
# include <strings.h>
# include "libft.h"
# include "env.h"
# include "parser.h"
# include "expansion/expansion.h"
# include "heredoc.h"
# include "executor.h"
# include "errors.h"

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
const char  *err_expansion_msg(int code);
const char  *err_heredoc_msg(int code);
t_error     err_create(t_err_domain domain, int code, t_err_payload payload);
const char 	*err_exec_msg(int code);

#endif