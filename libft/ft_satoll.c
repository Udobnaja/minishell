#include "libft.h"

static int	get_sign(char c, size_t *i)
{
	if (c == '+' || c == '-')
		*i = *i + 1;
	if (c == '-')
		return (-1);
	return (1);
}

static unsigned long long	set_limit(int sign)
{
	if (sign > 0)
		return ((unsigned long long)LLONG_MAX);
	return ((unsigned long long)LLONG_MAX + 1ULL);
}

static int	step_acc(
	unsigned long long *acc,
	unsigned long long limit,
	unsigned digit
)
{
	if (*acc > (limit - digit) / 10ULL)
		return (0);
	*acc = (*acc * 10ULL) + digit;
	return (1);
}

static long long	finalize_ll(unsigned long long acc, int sign)
{
	if (sign > 0)
		return ((long long)acc);
	return (-(long long)acc);
}

long long  ft_satoll(const char *nptr, int *error)
{
	int                 sign;
	unsigned long long  acc;
	unsigned long long  limit;
	size_t				i;
	unsigned			digit;

	acc = 0;
	i = 0;
	while (ft_isspace(nptr[i]))
		i++;
	sign = get_sign(nptr[i], &i);
	if (nptr[i] == '\0' || !ft_isdigit(nptr[i]))
		return ft_set_error(error);
	limit = set_limit(sign);
	while (nptr[i] && ft_isdigit(nptr[i]))
	{
		digit = (unsigned)(nptr[i] - '0');
		if (!step_acc(&acc, limit, digit))
			return (ft_set_error(error));
		i++;
	}
	if (nptr[i] != '\0')
		return ft_set_error(error);
	return (finalize_ll(acc, sign));
}