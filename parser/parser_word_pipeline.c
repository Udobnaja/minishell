#include "parser_internal.h"

static t_parser_status	prs_split_word(const char *arg, t_cmd *cmd);
static t_parser_status	prs_export_word(char *arg, t_cmd *cmd);

t_parser_status prs_word_to_argv(
	const t_word *word,
	t_shell *sh, t_cmd *cmd
)
{
	char			*arg;
	t_parser_status	status;
	int				is_export;

	arg = NULL;
	status = prs_join_word(word, sh, &arg);
	if (status != PARSE_OK)
		return (status);
	is_export = (cmd->argv && cmd->argv[0] && ft_strcmp(cmd->argv[0], "export") == 0);
	if (is_export && ft_strchr(arg, '=') != NULL)
		return (prs_export_word(arg, cmd));
	if (arg && ft_strchr(arg, FIELD_SEP))
	{
		status = prs_split_word(arg, cmd);
		free(arg);
		return (status);
	}
	if (!pipeline_push_cmd_argv(cmd, arg))
		return (PARSE_ALLOC_ERROR);
	return (PARSE_OK);
}

static t_parser_status	prs_export_word(char *arg, t_cmd *cmd)
{
	size_t i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] == FIELD_SEP)
			arg[i] = ' ';
		i++;
	}
	if (!pipeline_push_cmd_argv(cmd, arg))
		return (PARSE_ALLOC_ERROR);
	return (PARSE_OK);
}

static t_parser_status	prs_split_word(const char *arg, t_cmd *cmd)
{
	size_t	i;
	size_t	start;
	char	*piece;

	i = 0;
	while (arg[i])
	{
		while (arg[i] && arg[i] == FIELD_SEP)
			i++;
		start = i;
		while (arg[i] && arg[i] != FIELD_SEP)
			i++;
		if (i > start)
		{
			piece = ft_substr(arg, start, i - start);
			if (!piece)
				return (PARSE_ALLOC_ERROR);
			if (!pipeline_push_cmd_argv(cmd, piece))
				return (PARSE_ALLOC_ERROR);
		}
	}
	return (PARSE_OK);
}
