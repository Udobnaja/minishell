#ifndef EXPANSION_H
# define EXPANSION_H

# include "libft.h"
# include "env.h"

char	*expn_expand(char *str, const t_env_store *store, int last_status);
char	*expn_dup_env_key(const char *str);

#endif