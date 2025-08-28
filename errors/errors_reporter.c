#include "errors_internal.h"

static int	err_has_payload(const t_error *error);

void	err_print(t_err_domain domain, int code, t_err_payload payload)
{
	t_error error;

	error = err_create(domain, code, payload);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (err_has_payload(&error))
	{
		if (error.domain == ERR_LEXER && error.code == LEX_UNMATCHED_QUOTES)
			ft_eprintf(error.msg, error.payload.token);
		else if (error.domain == ERR_PARSER && error.code == PARSE_UNEXPECTED_TOKEN)  
			ft_eprintf(error.msg, error.payload.token);
		else
			ft_putstr_fd(error.msg, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);	
	}
	else    
		ft_putendl_fd(error.msg, STDERR_FILENO);
}

static int err_has_payload(const t_error *error)
{
	return (error->payload.token != NULL
		|| error->payload.path != NULL
		|| error->payload.command != NULL
		|| error->payload.errno_val != 0);
}