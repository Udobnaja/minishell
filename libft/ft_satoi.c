/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_satoi.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: audobnai <audobnai@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 14:11:01 by audobnai          #+#    #+#             */
/*   Updated: 2025/08/05 14:11:04 by audobnai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	has_overflow(int sign, long number);
static int	get_sign(char c);

int	ft_set_error(int *error)
{
	*error = 1;
	return (0);
}

int	ft_satoi(const char *nptr, int *error)
{
	int		sign;
	long	number;
	size_t	i;

	number = 0;
	i = 0;
	while (ft_isspace(nptr[i]))
		i++;
	sign = get_sign(nptr[i]);
	if ((nptr[i] == '+' || nptr[i] == '-'))
		i++;
	if (nptr[i] == '\0')
		return (ft_set_error(error));
	while (nptr[i] && ft_isdigit(nptr[i]))
	{
		number = (number * 10) + (nptr[i] - '0');
		if (has_overflow(sign, number))
			return (ft_set_error(error));
		i++;
	}
	if (nptr[i] != '\0')
		return (ft_set_error(error));
	return ((int)(number * sign));
}

static int	has_overflow(int sign, long number)
{
	if (sign == 1 && number > INT_MAX)
		return (1);
	else if (sign == -1 && (-number) < INT_MIN)
		return (1);
	return (0);
}

static int	get_sign(char c)
{
	if (c == '-')
		return (-1);
	return (1);
}
