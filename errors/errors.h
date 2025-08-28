#ifndef ERRORS_H
# define ERRORS_H

# include <stdio.h>

# include "../environment/env_internal.h"
# include "../libft/libft.h"

typedef struct s_error t_error;

typedef enum e_err_domain
{
	ERR_NONE = 0,
	ERR_ENV,
	ERR_PARSER,
	ERR_EXEC
}	t_err_domain;

typedef union u_err_payload
{
	int		errno_val;
	char	*token;
	char	*path;
	char	*command;
}	t_err_payload;


void	err_print(t_err_domain domain, int code, t_err_payload payload);

#endif