/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_functions.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavrukh <alavrukh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:34:28 by alavrukh          #+#    #+#             */
/*   Updated: 2025/08/12 12:24:55 by alavrukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_write_str(char *str)
{
	int	all_letter;
	int	letter;

	if (!str)
		return (write(1, "(null)", 6));
	all_letter = 0;
	letter = 0;
	while (*str)
	{
		letter = write(1, str, 1);
		if (letter == -1)
			return (-1);
		all_letter += letter;
		str++;
	}
	return (all_letter);
}

int	ft_write_base(unsigned long num, unsigned int base, char *str)
{
	int	count;
	int	letter;

	count = 0;
	letter = 0;
	if (num > base - 1)
		count = ft_write_base(num / base, base, str);
	if (count == -1)
		return (-1);
	letter = write(1, &str[num % base], 1);
	if (letter == -1)
		return (-1);
	count = count + letter;
	return (count);
}

int	ft_write_int(long num)
{
	int	sign;
	int	number;

	sign = 0;
	number = 0;
	if (num < 0)
	{
		sign = write(1, "-", 1);
		if (sign == -1)
			return (-1);
		num = -num;
	}
	number = ft_write_base(num, 10, BASE10);
	if (number == -1)
		return (-1);
	return (sign + number);
}

int	ft_write_ptr(void *ptr)
{
	int	ox;
	int	adress;

	if (!ptr)
		return (ft_write_str("(nil)"));
	ox = 0;
	adress = 0;
	ox = write(1, "0x", 2);
	adress = ft_write_base((unsigned long)ptr, 16, BASE16L);
	if (ox == -1)
		return (-1);
	if (adress == -1)
		return (-1);
	return (ox + adress);
}
