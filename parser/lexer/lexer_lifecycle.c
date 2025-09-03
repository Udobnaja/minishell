#include "lexer_internal.h"

t_token_list	*lex_create_token_list(void)
{
	t_token_list *s;

	s = ft_calloc(1, sizeof *s);
	return (s);
}

void	lex_clear_token_list(t_token_list *list)
{
	t_token_node	*cur;
	t_token_node	*next;

	if (!list)
		return;
	cur = list->head;
	while (cur)
	{
		next = cur->next;
		lex_free_node(cur);
		cur = next;
	}
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
}

void	lex_destroy_token_list(t_token_list **list)
{
	if (!list || !*list) return;
	lex_clear_token_list(*list);
	free(*list);
	*list = NULL;
}
