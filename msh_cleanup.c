/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_cleanup.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: audobnai <audobnai@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 21:44:28 by audobnai          #+#    #+#             */
/*   Updated: 2025/10/27 21:44:29 by audobnai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_internal.h"

void	msh_cleanup(t_shell *shell)
{
	if (!shell)
		return ;
	rl_clear_history();
	if (shell->heredoc_store)
		heredoc_store_destroy(&shell->heredoc_store);
	if (shell->env_store)
		env_destroy(&shell->env_store);
}

void	msh_clean_and_exit(t_shell *shell, int exit_status, int is_interactive)
{
	if (is_interactive)
		sh_termios_restore();
	msh_cleanup(shell);
	exit(exit_status);
}
