#include "errors_internal.h"

int	err_has_payload(const t_error *error)
{
	return (
		error->payload.token != NULL
		|| error->payload.identifier != NULL
		|| error->payload.path != NULL
		|| error->payload.command != NULL
		|| error->payload.errno_val != 0);
}

static void	err_putn(char *buf, size_t *pos, const char *str, size_t n)
{
	const size_t max_available = ERROR_MAX_BUFFER - *pos;

    if (*pos >= ERROR_MAX_BUFFER || n == 0)
		return;
    if (n > max_available)
		n = max_available;
	ft_memcpy(buf + *pos, str, n);
	*pos += n;
}

static void err_cpy_msg(char *buf, size_t *pos, const char *str)
{
	const size_t	str_len = ft_strlen(str);

	err_putn(buf, pos, str, str_len);
}

void err_format_msg(char *buf,  size_t *pos, const char *format, ...)
{
	va_list	args;
    int     i;
    char    *str;

	va_start(args, format);
    i = 0;
	while (format[i])
	{
		if (format[i] == '%' && format[i + 1] && format[i + 1] == 's')
		{
			i++;
            str = va_arg(args, char *);
			if (str)
				err_cpy_msg(buf, pos, str);
		}
		else
			err_putn(buf, pos,  &format[i], 1);
		i++;
	}
	va_end(args);
}