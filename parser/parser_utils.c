/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: audobnai <audobnai@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 21:50:45 by audobnai          #+#    #+#             */
/*   Updated: 2025/10/27 21:50:46 by audobnai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"

int	prs_is_simple_redirect(t_token_type type)
{
	return (type == T_REDIR_APP || type == T_REDIR_IN || type == T_REDIR_OUT);
}

int	prs_is_redirect(t_token_type type)
{
	return (type == T_HEREDOC || prs_is_simple_redirect(type));
}
