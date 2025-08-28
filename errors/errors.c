#include "errors.h"

void	err_print(const t_error *error)
{
    ft_putstr_fd("minishell: ", STDERR_FILENO);
    ft_putendl_fd(error->msg, STDERR_FILENO);
}