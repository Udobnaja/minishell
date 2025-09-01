#include "libft.h"

int	ft_eprintf(const char *format, ...)
{
	va_list	argc;
	int		count;
    int     i;
    char    *str;

	va_start(argc, format);
	count = 0;
    i = 0;
	while (format[i])
	{
		if (format[i] == '%' && format[i + 1] && format[i + 1] == 's')
		{
			i++;
            str = va_arg(argc, char *);
			if (str)
				count += write(STDERR_FILENO, str, ft_strlen(str));
		}
		else
			count += write(STDERR_FILENO, &format[i], 1);
		i++;
	}
	va_end(argc);
    return (count);
}