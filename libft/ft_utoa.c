/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: audobnai <audobnai@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 13:17:29 by audobnai          #+#    #+#             */
/*   Updated: 2025/05/13 13:17:31 by audobnai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_utoasize(unsigned long n)
{
	size_t	alloc_size;

	alloc_size = 0;
	if (n == 0)
		return (1);
	while (n)
	{
		alloc_size += 1;
		n /= 10;
	}
	return (alloc_size);
}

char	*ft_utoa(unsigned int un)
{
	size_t			alloc_size;
	char			*alloc_memory;
	unsigned long	unl;

	unl = (unsigned long)un;
	alloc_size = ft_utoasize(unl);
	alloc_memory = malloc((alloc_size + 1) * sizeof(char));
	if (!alloc_memory)
		return (NULL);
	alloc_memory[alloc_size] = '\0';
	if (unl == 0)
		alloc_memory[0] = '0';
	while (unl)
	{
		alloc_memory[--alloc_size] = (unl % 10) + '0';
		unl /= 10;
	}
	return (alloc_memory);
}
