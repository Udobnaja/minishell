#ifndef ERRORS_INTERNAL_H
# define ERRORS_INTERNAL_H

# include "errors.h"

typedef struct s_error
{
	t_err_domain	domain;
	int				code;
	t_err_payload	payload;
	char			msg[256];
}	t_error;

const char  *err_env_msg(int code);
t_error     err_create(t_err_domain domain, int code, t_err_payload payload);

#endif