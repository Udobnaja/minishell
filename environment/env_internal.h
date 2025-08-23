#ifndef ENV_INTERNAL_H
# define ENV_INTERNAL_H

# include <stdlib.h>
# include <string.h> // TODO: MSH-6
# include "env.h"


typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_env_store
{
	t_env	*head;
}	t_env_store;

t_env_status 	env_init_from_envp(t_env_store *store, char *const envp[]);
t_env_status	env_parse_row(const char *row, char **key, char **value);
t_env			*env_create_node(char *key, char *value);
void			env_push_node(t_env_store *store, t_env	*node);

#endif