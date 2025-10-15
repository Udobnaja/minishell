#include "parser_internal.h"

static t_builtin prs_cmd_to_builtin(const char *name);

t_parser_status prs_finish_cmd(t_cmd *cmd)
{
	char *default_name;

	if (!cmd->argv)
	{
		default_name = ft_strdup("");
		if (!default_name)
			return (PARSE_ALLOC_ERROR);
		if (!pipeline_push_cmd_argv(cmd, default_name))
			return (PARSE_ALLOC_ERROR);
	}
	cmd->name = ft_strdup(cmd->argv[0]);
	if (!cmd->name)
		return (PARSE_ALLOC_ERROR);
	cmd->builtin_kind = prs_cmd_to_builtin(cmd->name);	
	return (PARSE_OK);
}

static t_builtin prs_cmd_to_builtin(const char *name)
{
	if (!ft_strcmp(name, "echo"))
		return (BUILTIN_ECHO);
	if (!ft_strcmp(name, "cd"))
		return (BUILTIN_CD);
	if (!ft_strcmp(name, "pwd"))
		return (BUILTIN_PWD);
	if (!ft_strcmp(name, "export"))
		return (BUILTIN_EXPORT);
	if (!ft_strcmp(name, "unset"))
		return (BUILTIN_UNSET);
	if (!ft_strcmp(name, "env"))
		return (BUILTIN_ENV);
	if (!ft_strcmp(name, "exit"))
		return (BUILTIN_EXIT);
	return (BUILTIN_NONE);
}
