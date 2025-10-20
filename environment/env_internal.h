#ifndef ENV_INTERNAL_H
# define ENV_INTERNAL_H

# include "env.h"
# include "libft.h"
# include "utils.h"

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_env_store
{
	t_env			*head;
	size_t			size;
}					t_env_store;

t_env_status		env_init_from_envp(t_env_store *store, char *const envp[]);
t_env_status		env_empty_init(t_env_store *store);
t_env_status		env_parse_row(const char *row, char **key, char **value);
t_env				*env_create_node(char *key, char *value);
t_env				*env_lookup(const t_env_store *store, const char *key);
t_env_status		env_set_internal(t_env_store *store, char *key,
						char *value);
t_env_status		env_set_current_pwd(t_env_store *store);
t_env_status		env_set_shlvl(t_env_store *store, const char *lvl);
t_env_status		env_set_start_underscore(t_env_store *store,
						const char *name);
t_env_status		env_normalize(t_env_store *store);
void				env_push_node(t_env_store *store, t_env *node);
void				env_free_node(t_env *node);

#endif