#ifndef EXPANSION_H
# define EXPANSION_H

# include "libft.h"
# include "env.h"

typedef enum e_expansion_status {
	EXP_OK = 0,
	EXP_AMBIGUOUS_REDIRECT
}	t_expansion_status;

char	*expn_expand(char *str, const t_env_store *store, int last_status);

#endif