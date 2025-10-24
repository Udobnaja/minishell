/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: audobnai <audobnai@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 16:54:12 by audobnai          #+#    #+#             */
/*   Updated: 2025/05/03 18:18:12 by audobnai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static void	ft_strcpy(char *dst, const char *str, size_t n)
{
	size_t	i;

	i = 0;
	while (str[i] && (n == 0 || i < n))
	{
		dst[i] = str[i];
		i++;
	}
	dst[i] = '\0';
}

static char	*ft_strdup(const char *s)
{
	char	*str;
	size_t	len;

	len = ft_strlen(s);
	str = malloc((len + 1) * sizeof(char));
	if (!str)
		return (NULL);
	ft_strcpy(str, s, 0);
	return (str);
}

static char	*ft_strjoin(const char *s1, const char *s2, size_t s2_n)
{
	char	*alloc_memory;
	size_t	s1_len;
	size_t	s2_len;
	size_t	alloc_size;

	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	alloc_size = s1_len + s2_len + 1;
	alloc_memory = malloc(alloc_size * sizeof(char));
	if (!alloc_memory)
		return (NULL);
	ft_strcpy(alloc_memory, s1, 0);
	ft_strcpy(alloc_memory + s1_len, s2, s2_n);
	return (alloc_memory);
}

static char	*ft_merge_line_buf(char *line, char *buf, size_t n)
{
	char	*new_line;

	if (!line)
	{
		line = ft_strdup("");
		if (!line)
			return (NULL);
	}
	new_line = ft_strjoin(line, buf, n);
	if (!new_line)
	{
		free(line);
		return (NULL);
	}
	ft_strcpy(buf, buf + n, 0);
	free(line);
	return (new_line);
}

char	*get_next_line(int fd)
{
	static char	buf[BUFFER_SIZE + 1];
	char		*line;
	ssize_t		i;
	ssize_t		buf_size;

	line = NULL;
	buf_size = ft_strlen(buf);
	if (!buf_size)
		buf_size = read(fd, buf, BUFFER_SIZE);
	while (buf_size)
	{
		if (buf_size < 0)
			return (NULL);
		i = 0;
		buf[buf_size] = '\0';
		while (i < buf_size && buf[i] != '\n')
			i++;
		if (buf[i] == '\n')
			return (ft_merge_line_buf(line, buf, i + 1));
		line = ft_merge_line_buf(line, buf, i);
		if (!line)
			return (NULL);
		buf_size = read(fd, buf, BUFFER_SIZE);
	}
	return (line);
}