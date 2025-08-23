#ifndef ENV_H
# define ENV_H

typedef struct s_env_store t_env_store;

typedef enum e_env_status {
	ENV_OK = 0,
	ENV_ALLOC_ERROR
}	t_env_status;

t_env_store		*env_store_create(void);
t_env_status 	env_init(t_env_store *store, char *const envp[], const char *sh_name);
void			env_clear(t_env_store *store);
void			env_destroy(t_env_store **store);

t_env_status	env_set(t_env_store *s, const char *key, const char *value);

#endif