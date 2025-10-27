/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_internal.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: audobnai <audobnai@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 21:59:19 by audobnai          #+#    #+#             */
/*   Updated: 2025/10/27 21:59:20 by audobnai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_INTERNAL_H
# define HEREDOC_INTERNAL_H

# include <stdint.h>
# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <sys/errno.h>

# include "heredoc.h"
# include "libft.h"
# include "../expansion/expansion.h"

# define HEREDOC_STORE_CAPACITY 16

t_heredoc_status	heredoc_write_line(int fd, const char *document,
						int has_expansion, t_shell *sh);
int					heredoc_write(int fd, const char *buf, size_t len);
int					heredoc_read_prompt(char *line, t_heredoc_status *status,
						char *eof);

#endif