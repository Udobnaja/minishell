#include "lexer_internal.h"

void	lex_push_node(t_token_list *list, t_token_node *node)
{
	if (!list->head)
	{
		list->head = node;
		list->tail = node;
	}
	else {
		list->tail->next = node;
		list->tail = node; 
	}
	list->size++;
}

t_token_node	*lex_create_node(t_token *token)
{
	t_token_node	*node;

	node = malloc(sizeof(t_token_node));
	if (!node)
		return (NULL);
	node->token = token;
	node->next = NULL;
	return (node);
}