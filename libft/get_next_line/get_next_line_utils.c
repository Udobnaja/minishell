/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavrukh <alavrukh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:39:22 by alavrukh          #+#    #+#             */
/*   Updated: 2025/08/12 12:25:02 by alavrukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*dest;
	size_t	i;
	size_t	len;

	len = ft_strlen(s);
	dest = (char *)malloc (len + 1);
	if (!dest)
		return (NULL);
	i = 0;
	while (s[i])
	{
		dest[i] = s[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strchr(const char *s, int c)
{
	unsigned char		chr;
	unsigned const char	*str;

	chr = (unsigned char)c;
	str = (unsigned const char *)s;
	while (*str)
	{
		if (*str == chr)
			return ((char *)str);
		str++;
	}
	if (*str == chr)
		return ((char *)str);
	return (NULL);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len_total;
	char	*joined;
	char	*temp;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	len_total = ft_strlen(s1) + ft_strlen(s2);
	temp = (char *)malloc(sizeof(char) * (len_total + 1));
	if (temp == NULL)
		return (NULL);
	joined = temp;
	while (*s1)
		*temp++ = *s1++;
	while (*s2)
		*temp++ = *s2++;
	*temp = '\0';
	return (joined);
}

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		i++;
	}
	return (i);
}
