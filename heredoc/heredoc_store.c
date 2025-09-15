#include "heredoc_internal.h"

static void heredoc_cpy(t_heredoc_entry	*entries, const t_heredoc_store *src_store);

int	heredoc_store_add(t_heredoc_store *store, int fd)
{
	t_heredoc_entry	*entries;
	size_t			capacity;

	if (store->capacity == 0 || store->count >= store->capacity)
	{
		if (store->capacity == 0)
			capacity = 16;
		else
			capacity = store->capacity * 2;
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