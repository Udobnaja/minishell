/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavrukh <alavrukh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 13:59:29 by alavrukh          #+#    #+#             */
/*   Updated: 2025/08/12 12:25:16 by alavrukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
// libraries

# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>

# define BASE16L "0123456789abcdef"
# define BASE16U "0123456789ABCDEF"
# define BASE10 "0123456789"

// mandatory functions
int		ft_isalpha(int c);
int		ft_isdigit(int c);
int		ft_isalnum(int c);
int		ft_isascii(int c);
int		ft_isprint(int c);
int		ft_isspace(int c);
int		ft_tolower(int c);
int		ft_toupper(int c);
int		ft_atoi(const char *nptr);
int		ft_satoi(const char *nptr, int *error);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
int		ft_putchar_fd(char c, int fd);
char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
char	*ft_strnstr(const char *big, const char *little, size_t len);
char	*ft_strdup(const char *s);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strtrim(char const *s1, char const *set);
char	**ft_split(char const *s, char c);
char	*ft_itoa(int n);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void	ft_striteri(char *s, void (*f)(unsigned int, char *));
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(const char *s, int fd);
void	ft_putnbr_fd(int n, int fd);
void	ft_bzero(void *s, size_t n);
void	*ft_memset(void *s, int c, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	*ft_memmove(void *dest, const void *src, size_t n);
void	*ft_memchr(const void *s, int c, size_t n);
void	*ft_calloc(size_t nmemb, size_t size);
size_t	ft_strlen(const char *str);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
size_t	ft_strlcat(char *dst, const char *src, size_t size);
// free split
void	*ft_free(char **res);

// print f
int		ft_printf(const char *str, ...);
int		ft_format(const char *str, va_list *argc);
int		ft_write_ptr(void *ptr);
int		ft_write_int(long num);
int		ft_write_base(unsigned long num, unsigned int base, char *str);
int		ft_write_str(char *str);

// get_next_line
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 55
# endif

char	*get_next_line(int fd);
char	*fill_line(int fd, char *remainder, char *reading);
char	*save_line(char *remainder);

void	clean_remainder(void);

#endif