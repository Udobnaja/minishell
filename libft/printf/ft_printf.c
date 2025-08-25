/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavrukh <alavrukh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 14:16:53 by alavrukh          #+#    #+#             */
/*   Updated: 2025/08/12 12:24:50 by alavrukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_format(const char *str, va_list *argc)
{
	if (*str == '%')
		return (ft_putchar_fd('%', 1));
	if (*str == 'c')
		return (ft_putchar_fd(va_arg(*argc, int), 1));
	if (*str == 's')
		return (ft_write_str(va_arg(*argc, char *)));
	if (*str == 'u')
		return (ft_write_base(va_arg(*argc, unsigned int), 10, BASE10));
	if (*str == 'x')
		return (ft_write_base(va_arg(*argc, unsigned int), 16, BASE16L));
	if (*str == 'X')
		return (ft_write_base(va_arg(*argc, unsigned int), 16, BASE16U));
	if (*str == 'i' || *str == 'd')
		return (ft_write_int(va_arg(*argc, int)));
	if (*str == 'p')
		return (ft_write_ptr(va_arg(*argc, void *)));
	return (-1);
}

int	ft_printf(const char *str, ...)
{
	va_list	argc;
	int		letter;
	int		count;

	if (!str)
		return (-1);
	va_start(argc, str);
	count = 0;
	letter = 0;
	while (*str)
	{
		if (*str == '%')
		{
			str++;
			letter = ft_format(str, &argc);
		}
		else
			letter = write(1, str, 1);
		if (letter == -1)
			return (va_end(argc), -1);
		count = count + letter;
		str++;
	}
	va_end(argc);
	return (count);
}
