/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavrukh <alavrukh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 12:01:41 by alavrukh          #+#    #+#             */
/*   Updated: 2025/05/02 16:26:02 by alavrukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
//What ft_strmapi does is apply the function f 
// to every character of the string s.

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*smodified;
	size_t	i;

	if (!s || !f)
		return (NULL);
	i = 0;
	smodified = malloc(ft_strlen(s) + 1);
	if (!smodified)
		return (NULL);
	while (s[i])
	{
		smodified[i] = f(i, s[i]);
		i++;
	}
	smodified[i] = '\0';
	return (smodified);
}
