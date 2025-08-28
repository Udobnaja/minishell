#ifndef ERRORS_H
# define ERRORS_H

# include <stdio.h>

# include "../environment/env_internal.h"
# include "../libft/libft.h"

typedef enum e_err_domain {
	ERR_NONE = 0,
	ERR_ENV,
	ERR_PARSER,
	ERR_EXEC
}	t_err_domain;

typedef struct s_error {
	t_err_domain	domain;
	int				code; 
	char			msg[256];
}	t_error;

void	err_print(const t_error *error);
t_error	err_create(t_err_domain domain, int code);

// private

const char *err_env_msg(int code);

#endif