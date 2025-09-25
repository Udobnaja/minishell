#include "heredoc_internal.h"

static void heredoc_cpy(t_heredoc_entry	*entries, const t_heredoc_store *src_store);

t_heredoc_store *heredoc_store_create(void)
{
	t_heredoc_store *s;

	s = ft_calloc(1, sizeof *s);
	return s;
}

void heredoc_store_clear(t_heredoc_store *store)
{
    size_t i;

    if (!store)
        return;
    i = 0;
    while (i < store->count)
    {
        if (store->entries[i].fd >= 0)
            close(store->entries[i].fd);
        i++;
    }
    free(store->entries);
    store->entries = NULL;
    store->count = 0;
    store->capacity = 0;
}

int	heredoc_store_add(t_heredoc_store *store, int fd)
{
	t_heredoc_entry	*entries;
	size_t			capacity;

	if (store->capacity == 0 || store->count >= store->capacity)
	{
		if (store->capacity == 0)
			capacity = HEREDOC_STORE_CAPACITY;
		else
		{
			if (store->capacity > SIZE_MAX / 2)
        		return (-1);
			capacity = store->capacity * 2;
		}			
		entries = ft_calloc(capacity, sizeof(t_heredoc_entry));
		if (!entries)
			return (-1);
		heredoc_cpy(entries, store);
		free(store->entries);
		store->entries = entries;
		store->capacity = capacity;
	}
	store->entries[store->count].fd = fd;
	store->count++;
	return (0);
}

static void heredoc_cpy(t_heredoc_entry	*entries, const t_heredoc_store *src_store)
{
	size_t	i;

	i = 0;
	while (i < src_store->count)
	{
		entries[i] = src_store->entries[i];
		i++;
	}
}

void heredoc_store_destroy(t_heredoc_store **store)
{
	if (!store || !*store) return;
	heredoc_store_clear(*store);
	free(*store);
	*store = NULL;
}
