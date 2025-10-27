/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: audobnai <audobnai@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 22:45:09 by audobnai          #+#    #+#             */
/*   Updated: 2025/10/27 22:45:10 by audobnai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

# include "env.h"
# include "libft.h"

char	*expn_expand(char *str, const t_env_store *store, int last_status);
char	*expn_dup_env_key(const char *str);

#endif