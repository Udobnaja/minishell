/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavrukh <alavrukh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:14:03 by alavrukh          #+#    #+#             */
/*   Updated: 2025/05/02 16:23:00 by alavrukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	totalbytes;
	void	*ptr;

	totalbytes = nmemb * size;
	if (size && (totalbytes / size) != nmemb)
		return (NULL);
	ptr = malloc(totalbytes);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, totalbytes);
	return (ptr);
}
