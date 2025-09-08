#ifndef ERRORS_H
# define ERRORS_H

# include <stdio.h>

# include "libft.h"
# include "env.h"
# include "parser.h"
# include "expansion/expansion.h"
# include "heredoc/heredoc.h"
# include "executor.h"

typedef struct s_error t_error;

typedef enum e_err_domain
{
	ERR_NONE = 0,
	ERR_ENV,
	ERR_LEXER,
	ERR_PARSER,
	ERR_EXPANSION,
	ERR_HEREDOC,
	ERR_EXEC
}	t_err_domain;

typedef union u_err_payload
{
	int			errno_val;
	const char	*token;
	char		*path;
	char		*command;
}	t_err_payload;


void	err_print(t_err_domain domain, int code, t_err_payload payload);

#endif