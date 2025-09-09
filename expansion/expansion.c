#include "expansion.h"


char	*expn_expand(char *str, const t_env_store *store, int last_status)
{
	const char *value;

	if (str[1] == '\0')
		return ft_strdup(str);
	if (str[1] == '?' && str[2] == '\0')
		return ft_itoa(last_status);
	if (env_key_is_valid(str + 1))
	{
		value = env_get_value(store, str + 1);
		if (!value)
			return ft_strdup("");
		else
			return ft_strdup(value);   
	}
	return ft_strdup(str);     
}