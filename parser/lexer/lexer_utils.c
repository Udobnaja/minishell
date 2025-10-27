/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: audobnai <audobnai@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 21:50:02 by audobnai          #+#    #+#             */
/*   Updated: 2025/10/27 21:50:03 by audobnai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"

void	lex_push_node(t_token_list *list, t_token_node *node)
{
	if (!list->head)
	{
		list->head = node;
		list->tail = node;
	}
	else
	{
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

void	lex_free_node(t_token_node	*node)
{
	lex_free_token(node->token);
	free(node);
}
