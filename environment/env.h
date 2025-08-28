#ifndef ENV_H
# define ENV_H

#include <stddef.h> 

typedef struct s_env_store t_env_store;

typedef enum e_env_status {
	ENV_OK = 0,
	ENV_ALLOC_ERROR
}	t_env_status;

typedef struct {
	char *key;
	char *value;
}	t_env_pair;

t_env_store		*env_store_create(void);
t_env_status 	env_init(t_env_store *store, char *const envp[], const char *sh_name);
void			env_clear(t_env_store *store);
void			env_destroy(t_env_store **store);

size_t			env_size(const t_env_store *store);

t_env_status	env_set(t_env_store *store, const char *key, const char *value);
void			env_unset(t_env_store *store, const char *key);
const char 		*env_get_value(const t_env_store *store, const char *key);

char			**env_to_envp(const t_env_store *store);
t_env_pair		*env_pairs_dup(const t_env_store *store);
void			env_pairs_free(t_env_pair *pairs, const size_t size);
void			env_free_envp(char **envp);


#endif