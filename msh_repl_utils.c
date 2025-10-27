/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_repl_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: audobnai <audobnai@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 21:45:06 by audobnai          #+#    #+#             */
/*   Updated: 2025/10/27 21:45:07 by audobnai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_internal.h"

const char	*msh_get_prompt(const char *name)
{
	static char		buf[PROMPT_MAX];
	const size_t	max_name = PROMPT_MAX - 4;
	const char		*slash = ft_strrchr(name, '/');
	const char		*base = name;
	size_t			n;

	if (slash != NULL)
		base = slash + 1;
	n = ft_strlen(base);
	if (n > max_name)
		n = max_name;
	ft_memcpy(buf, base, n);
	ft_memcpy(buf + n, " $ ", 4);
	return (buf);
}

int	msh_has_only_spaces(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isspace(str[i]))
			return (0);
		i++;
	}
	return (1);
}

const char	*msh_get_name(int argc, char **argv)
{
	if (argc > 0 && argv && argv[0] && argv[0][0] != '\0')
		return (argv[0]);
	return (SHELL_NAME);
}

void	msh_run_pipeline(char *line, t_shell *sh, int is_interactive)
{
	t_pipeline			pipeline;
	t_msh_parse_result	parse_result;
	t_exec_result		exec_result;

	ft_bzero(&pipeline, sizeof pipeline);
	ft_bzero(&exec_result, sizeof exec_result);
	parse_result = msh_parse(line, sh, &pipeline);
	if (parse_result.domain == MPR_OK)
	{
		exec_result = execute(sh, &pipeline);
		sh->last_status = exec_result.exit_code;
		heredoc_store_clear(sh->heredoc_store);
		if (exec_result.flow == FLOW_EXIT)
		{
			free(line);
			pipeline_destroy(&pipeline);
			msh_clean_and_exit(sh, sh->last_status, is_interactive);
		}
	}
	else
		sh->last_status = msh_parse_result_to_exit_status(parse_result);
	pipeline_destroy(&pipeline);
	free(line);
}

int	msh_signal_check(char *line, t_shell *sh)
{
	if (g_last_signal == SIGINT)
	{
		sh->last_status = sh_status_from_signal(SIGINT);
		g_last_signal = 0;
		free(line);
		return (1);
	}
	return (0);
}
